#include "SocketServer.hpp"
#include <unistd.h>
#include <exception>
#include <string.h>
#include <netdb.h>
#include <cstdio>
#include <iostream>
#include "utils.hpp"
#include "config.hpp"

SocketServer::SocketServer(int domain, int type, int protocol) :
	_fd(-1), _domain(domain), _type(type), _protocol(protocol)
{
}

SocketServer::~SocketServer()
{
	this->close();
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
		throw std::exception();
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = _domain;
	hints.ai_socktype = _type;
	hints.ai_flags = AI_PASSIVE;

	(void)port;
	status = getaddrinfo(NULL, /*ITOA(port)*/"12345", &hints, &servinfo);
	if (status != 0)
	{
		std::exception(/* gai_strerror(status) */);
	}

	if (::bind(_fd, servinfo->ai_addr, servinfo->ai_addrlen) < 0)
	{
		freeaddrinfo(servinfo);
		throw std::exception(/*perror("")*/);
	}
	freeaddrinfo(servinfo);
}

void	SocketServer::listen()
{
	if (::listen(_fd, BACKLOG) == -1)
	{
		perror("listen() failed");
		throw std::exception();
	}
}

void	SocketServer::close()
{
	if (_fd < 0)
	{
		::close(_fd);
		_fd = -1;
	}
}