/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPStream.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <lefendie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:58:26 by lefendie          #+#    #+#             */
/*   Updated: 2022/11/02 09:15:16 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <TCPStream.hpp>

#include <sys/epoll.h>
#include <netinet/ip.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sys/socket.h>

TCPStream::TCPStream(): _fd(-1), _events(0), _triggered_events(0)
{
}

TCPStream::TCPStream(const int fd, socklen_t addrlen, const uint32_t events)
  : _fd(fd), _ip(fd, addrlen), _events(events), _triggered_events(0)
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
		_ip = rhs._ip;
		_events = rhs._events;
		_triggered_events = rhs._triggered_events;
		_message = rhs._message;
	}

	return (*this);
}

TCPStream::TCPStream(const TCPStream &src): _fd(src._fd), _ip(src._ip),
  _events(src._events), _triggered_events(src._triggered_events),
  _message(src._message)
{
}

void TCPStream::registerEvent(const uint32_t event)
{
  _events |= event;
}

void  TCPStream::resetEvents()
{
  _events = 0;
}

void TCPStream::setMessage(const std::string &message)
{
	_message = message;
}

int TCPStream::getFd() const
{
	return (_fd);
}

const IPAddress& TCPStream::getIPAddr() const
{
	return (_ip);
}

uint32_t TCPStream::triggeredEvents() const
{
	return _triggered_events;
}

const std::string& TCPStream::getMessage() const
{
	return (_message);
}

std::ostream &operator<<(std::ostream &out, const TCPStream &stream)
{
	out << "Client fd: " << stream.getFd()
    << ", IPAddress: " << stream.getIPAddr().toStr() << ", Event: ";
	if (stream.triggeredEvents() & EPOLLIN)
		out << " EPOLLIN ";
	if (stream.triggeredEvents() & EPOLLOUT)
		out << " EPOLLOUT ";
	out << ", Message: " << stream.getMessage();
//	out << ", Data: \n- Port: " << (uint16_t)ntohs((stream.getData()).sin_port);
//	out << "\n- Address: " << inet_ntoa(stream.getData().sin_addr) << '\n';
	return (out);
}
