/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPStream.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <lefendie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:58:26 by lefendie          #+#    #+#             */
/*   Updated: 2022/10/28 12:00:05 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <TCPStream.hpp>

#include <arpa/inet.h>
#include <sys/epoll.h>
#include <string>
#include <stdexcept>

TCPStream::TCPStream(int fd, sockaddr_in data): _fd(fd), _data(data), _event(-1), _message("")
{
	
}

TCPStream::~TCPStream()
{

}

TCPStream &TCPStream::operator=(const TCPStream &rhs)
{
	_event = rhs._event;
	_message = rhs._message;

	return (*this);
}

TCPStream::TCPStream(const TCPStream &src)
{
	_event = src._event;
	_message = src._message;
}

void TCPStream::setEvent(int event)
{
	if (event == EPOLLIN || event == EPOLLOUT)
	_event = event;
	else
		throw std::invalid_argument("Only use EPOLLIN or EPOLLOUT");
}

void TCPStream::setMessage(std::string message)
{
	_message = message;
}
