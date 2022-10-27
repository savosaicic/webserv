#include <TCPListener.hpp>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

TCPListener::TCPListener() : _epfd(-1)
{
}

TCPListener::~TCPListener()
{
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

    for (std::vector<std::pair<int, const char*> >::iterator it = _fd_port_list.begin(); it != _fd_port_list.end(); ++it)
    {
        if (::listen(it->first, 10) == -1)
            throw std::runtime_error("listen() failed");
				events.data.fd = it->first;
        if (epoll_ctl(_epfd, EPOLL_CTL_ADD, it->first, &events) == -1)
        {
            // close fds added in epoll list
            throw std::runtime_error("epoll_ctl() failed");
        }
    }
}

bool TCPListener::isListener(int fd)
{
    for (fd_port_t::iterator it = _fd_port_list.begin();
				it != _fd_port_list.end(); ++it)
		{
			if (fd == it->first)
				return true;
		}
		return false;
}

void TCPListener::incoming()
{
	int ready_fd_count;
  struct epoll_event events[MAX_EVENT];
	struct sockaddr_in client_addr;
	socklen_t addr_len;
	int fd;

	fd = 0;
	memset(&events, 0, sizeof(events));
  ready_fd_count = epoll_wait(_epfd, &events[0], MAX_EVENT, EPOLL_TIMEOUT);
		
	for(int i = 0; i < ready_fd_count; i++)
	{
		fd = events[i].data.fd;	
		if (isListener(fd))
		{
			int client = accept(fd, (sockaddr*)&client_addr, &addr_len);
			
			char *str = inet_ntoa(client_addr.sin_addr);
			std::cout << "Accepted: " << str << '\n';
			close(client);
			//new connection and accept() and create stream and set the corresponding event 
		}
		else
		{
			//
		}
	}
}
