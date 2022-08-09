#include "TCPServer.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include "utils.hpp"
#include <stdexcept>
#include <string>
#include <string.h>
#include <errno.h>

#ifdef DEBUG
#include "debug.hpp"
#endif

TCPServer::TCPServer() : SocketServer(AF_INET, SOCK_STREAM, 0)
{
}

TCPServer::~TCPServer()
{
	_connections.clear();
}

void	TCPServer::listen(int port, SA_IN server_addr)
{
	this->bind(port);
	SocketServer::listen();
	(void)server_addr;
}

void	TCPServer::accept()
{
	SA_IN		client_addr;
	socklen_t	addr_size;
	
	addr_size = sizeof(client_addr);
	
	int newfd = ::accept(_fd, (SA *)&client_addr, &addr_size);
	if (newfd < 0)
	{
		throw std::runtime_error(std::string(strerror(errno)));
	}

	
	_connections.push_back(TCPConnection(newfd, client_addr));
	
#ifdef DEBUG
	accepted_connection_from((char *)&client_addr.sin_addr);
#endif
}
