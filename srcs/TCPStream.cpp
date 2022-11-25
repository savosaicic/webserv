#include <TCPStream.hpp>
#include <sys/epoll.h>
#include <netinet/ip.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

TCPStream::TCPStream(): _fd(-1), _events(0), _triggered_events(0)
{
}

TCPStream::TCPStream(const int fd, const struct sockaddr remote_addr,
		const socklen_t remote_addrlen, const struct sockaddr host_addr,
		const socklen_t host_addrlen, const uint32_t events)
  : _fd(fd), _host_addr(host_addr, host_addrlen),
  _remote_addr(remote_addr, remote_addrlen), _events(events),
  _triggered_events(0)
{
}

TCPStream::~TCPStream()
{
}

TCPStream &TCPStream::operator=(const TCPStream &rhs)
{
	if (this != &rhs)
	{
		_fd = rhs._fd;
    _host_addr = rhs._host_addr;
		_remote_addr = rhs._remote_addr;
		_events = rhs._events;
		_triggered_events = rhs._triggered_events;
		_in_msg = rhs._in_msg;
		_out_msg = rhs._out_msg;
	}

	return (*this);
}

TCPStream::TCPStream(const TCPStream &src): _fd(src._fd),
  _host_addr(src._host_addr), _remote_addr(src._remote_addr),
  _events(src._events), _triggered_events(src._triggered_events),
  _in_msg(src._in_msg), _out_msg(src._out_msg)
{
}

void TCPStream::registerEvent(const uint32_t event)
{
  _events |= event;
}

void TCPStream::unRegisterEvent(const uint32_t event)
{
  _events &= ~event;
}

void  TCPStream::resetEvents()
{
  _events = 0;
}

void  TCPStream::setTriggeredEvents(const uint32_t events)
{
  _triggered_events = events;
}

int TCPStream::getFd() const
{
	return _fd;
}

const IPAddress& TCPStream::getIPAddr() const
{
	return _remote_addr;
}

const IPAddress& TCPStream::getHostIPAddr() const
{
	return _host_addr;
}

bool TCPStream::canRead() const
{
	return _triggered_events & EPOLLIN;
}

bool TCPStream::canWrite() const
{
	return _triggered_events & EPOLLOUT;
}

uint32_t TCPStream::getRegisteredEvents() const
{
  return _events;
}

const std::string& TCPStream::getInMessage() const
{
	return _in_msg;
}

void  TCPStream::setOutMessage(const std::string &str)
{
  _out_msg = str;
}

void  TCPStream::clearInMessage()
{
	_in_msg.clear();
}

void  TCPStream::clearOutMessage()
{
	_out_msg.clear();
}

const std::string& TCPStream::getOutMessage() const
{
	return _out_msg;
}

int TCPStream::read()
{
  char  buffer[8192];
  int   bytes_read;

  memset(&buffer, 0, sizeof(buffer));
  bytes_read = recv(_fd, &buffer, sizeof(buffer), 0);
  _in_msg = std::string(buffer, bytes_read);
  _triggered_events &= ~EPOLLIN;
  return bytes_read;
}

int TCPStream::write(const std::string& str)
{
  const char  *buffer;
  int len;
  int bytes_sent = 0;

  buffer = str.data();
  len = str.size();
  while (bytes_sent < len)
  {
    bytes_sent += send(_fd, buffer + bytes_sent, len - bytes_sent, 0);
  }
  _triggered_events &= ~EPOLLOUT;
  return bytes_sent;
}

bool  TCPStream::operator==(const TCPStream& rhs)
{
  return _fd == rhs._fd;
}

std::ostream &operator<<(std::ostream &out, const TCPStream &stream)
{
	out << "Client fd: " << stream.getFd()
    << ", IPAddress: " << stream.getIPAddr().toStr() << ", Event: ";
	if (stream.canRead())
		out << " can read ";
	if (stream.canWrite())
		out << " can write ";
//	out << ", Data: \n- Port: " << (uint16_t)ntohs((stream.getData()).sin_port);
//	out << "\n- Address: " << inet_ntoa(stream.getData().sin_addr) << '\n';
	return (out);
}
