#include <TCPListener.hpp>
#include <algorithm>
#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <utility>
#include <stdint.h>
#include <TCPStream.hpp>
#include <vector>
#include <ws_syscall_error.hpp>


TCPListener::TCPListener() : _epfd(-1), _streams_manager(_epfd, *this)
{
}

TCPListener::TCPListener(const TCPListener &src) : _epfd(src._epfd),
  _fd_port_list(src._fd_port_list), _streams_manager(src._streams_manager)
{
}

TCPListener::~TCPListener()
{
}

TCPListener TCPListener::operator=(const TCPListener &rhs)
{
  if (this != &rhs)
  {
    _epfd = rhs._epfd;
    _fd_port_list = rhs._fd_port_list;
    _streams_manager = rhs._streams_manager;
  }
  return *this;
}

void TCPListener::bind(const char* ip, const char* port)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int yes = 1;

	if (!ip || !port)
		throw std::invalid_argument("TCPListener::bind: ip or port is NULL");

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICHOST;

	if (getaddrinfo(ip, port, &hints, &result) != 0)
		throw std::runtime_error("getaddrinfo() failed");

	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		int fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (fd == -1)
			continue;
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
			throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
		if (::bind(fd, rp->ai_addr, rp->ai_addrlen) == 0)
		{
			_fd_port_list.push_back(std::make_pair(fd, port));
			break;
		}
		::close(fd);
	}
	if (!rp)
		throw std::runtime_error("bind() failed");
	freeaddrinfo(result);
}

void TCPListener::listen()
{
	struct epoll_event events;

	if (_fd_port_list.begin() == _fd_port_list.end())
		throw std::runtime_error("No socket to listen on");

	memset(&events, 0, sizeof(events));
	events.events = EPOLLIN;
	_epfd = epoll_create1(0);
	if (_epfd == -1)
		throw std::runtime_error("epoll_create1() failed");

	for (std::vector<std::pair<int, const char*> >::iterator it
      = _fd_port_list.begin(); it != _fd_port_list.end(); ++it)
	{
		if (::listen(it->first, 10) == -1)
			throw std::runtime_error("listen() failed");
#ifdef WS_DEBUG
    std::cerr << "Listening on port: " << it->second << std::endl;
#endif
		events.data.fd = it->first;
		events.events = EPOLLIN;
		if (epoll_ctl(_epfd, EPOLL_CTL_ADD, it->first, &events) == -1)
		{
			// close fds added in epoll list
			throw std::runtime_error("epoll_ctl() failed");
		}
	}
}

bool TCPListener::isListener(int fd)
{
	for (std::vector<std::pair<int, const char*> >::iterator it = _fd_port_list.begin();
			it != _fd_port_list.end(); ++it)
	{
		if (fd == it->first)
			return true;
	}
	return false;
}

std::vector<TCPStreamManager::iterator>  TCPListener::incoming()
{
  struct epoll_event  events[MAX_EVENT];
	struct sockaddr     client_addr;
	socklen_t           addrlen;
  int                 ready;

  memset(&events, 0, sizeof(events));
  ready = epoll_wait(_epfd, &events[0], MAX_EVENT, EPOLL_TIMEOUT);

  std::vector<TCPStreamManager::iterator> incoming_connection;

	for(int i = 0; i < ready; i++)
	{
		int fd = events[i].data.fd;
		if (isListener(fd))
		{
			memset(&client_addr, 0, sizeof(client_addr));
      addrlen = sizeof(client_addr);
			int client = accept4(fd, &client_addr, &addrlen, SOCK_NONBLOCK);

      struct epoll_event  event;
			memset(&event, 0, sizeof(event));
      event.data.fd = client;
      event.events = EPOLLIN;
      epoll_ctl(_epfd, EPOLL_CTL_ADD, client, &event);

      struct sockaddr our_addr;
      socklen_t our_addrlen = sizeof(our_addr);
      if (getsockname(fd, &our_addr, &our_addrlen) < 0)
        throw ws_syscall_error("getsockname()");

      _streams_manager.addClient(
          TCPStream(client, client_addr, addrlen, our_addr, our_addrlen, EPOLLIN)
      );
		}
		else
		{
      TCPStreamManager::iterator it = _streams_manager.getByFd(fd);
      it->second.setTriggeredEvents(events[i].events);
	    incoming_connection.push_back(it);
		}
	}
	return incoming_connection;
}

void  TCPListener::dropClient(const TCPStream& client)
{
  _streams_manager.dropClient(client);
}

void  TCPListener::registerClientEvent(TCPStream& client, const uint32_t event)
{
  _streams_manager.registerClientEvent(client, event);
}

void  TCPListener::unRegisterClientEvent(TCPStream& client,
    const uint32_t event)
{
  _streams_manager.unRegisterClientEvent(client, event);
}
