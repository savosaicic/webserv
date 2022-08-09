#include "TCPConnection.hpp"
#include <iostream>
#include <unistd.h>

TCPConnection::TCPConnection() : _fd(-1)
{
	(void)_fd;
	(void)_io_events;
	(void)_addr;
}

TCPConnection::TCPConnection(int connection_fd, const SA_IN &addr) : _fd(connection_fd), _addr(addr)
{
	(void)_fd;
	(void)_io_events;
	(void)_addr;
}

TCPConnection::TCPConnection(const TCPConnection &src) : _fd(src._fd)
{
}

TCPConnection::~TCPConnection()
{
//	this->close();
}

void	TCPConnection::close()
{
	if (_fd > -1)
	{
		::close(_fd);
		_fd = -1;
	}
}
