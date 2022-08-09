#include "SocketServer.hpp"
#include <cerrno>
#include <unistd.h>
#include <exception>
#include <string.h>
#include <netdb.h>
#include <cstdio>
#include <iostream>
#include "utils.hpp"
#include "config.hpp"

#include <stdexcept>

SocketServer::SocketServer(int domain, int type, int protocol) :
	_fd(-1), _domain(domain), _type(type), _protocol(protocol)
{
}

SocketServer::~SocketServer()
{
//	this->close();
}

void	SocketServer::bind(const int port)
{
	struct addrinfo	hints;
	struct addrinfo	*servinfo;
	int				status;
	
	//check if _fd is already binded
	// if (fcntl(fd, F_GETFD) == -1)
	
	// Maybe create the socket in a member funtion create()
	_fd = socket(_domain, _type, _protocol);
	if (_fd < 0)
		throw ws_exception(error_message("socket()", strerror(errno)));
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = _domain;
	hints.ai_socktype = _type;
	hints.ai_flags = AI_PASSIVE;

	(void)port;
	status = getaddrinfo(NULL, /*ITOA(port)*/"8042", &hints, &servinfo);
	if (status != 0)
		throw ws_exception(error_message("getaddrinfo()", gai_strerror(status)));

	if (::bind(_fd, servinfo->ai_addr, servinfo->ai_addrlen) < 0)
	{
		freeaddrinfo(servinfo);
		throw ws_exception(error_message("bind()", strerror(errno)));
	}
	freeaddrinfo(servinfo);
}

void	SocketServer::listen()
{
	if (::listen(_fd, BACKLOG) == -1)
		throw ws_exception(error_message("listen()", strerror(errno)));
}

void	SocketServer::close()
{
	if (_fd > -1)
	{
		::close(_fd);
		_fd = -1;
	}
}
