/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPStream.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <lefendie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:58:26 by lefendie          #+#    #+#             */
/*   Updated: 2022/11/01 15:47:20 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <TCPStream.hpp>

#include <sys/epoll.h>
#include <netinet/ip.h>
#include <string>
#include <stdexcept>
#include <iostream>

TCPStream::TCPStream(): _fd(-1), _event(0)
{
	
}

TCPStream::TCPStream(int fd, IPAddress data, uint32_t event): _fd(fd), _data(data), _event(event)
{
	std::cout << "File descriptor: " << fd << '\n';
}

TCPStream::~TCPStream()
{

}

TCPStream &TCPStream::operator=(const TCPStream &rhs)
{
	_fd = rhs._fd;
	_data = rhs._data;
	_event = rhs._event;
	_message = rhs._message;

	return (*this);
}

TCPStream::TCPStream(const TCPStream &src): _fd(src._fd), _data(src._data), _event(src._event), _message(src._message)
{
}

void TCPStream::setEvent(const uint32_t event)
{
	if (event == EPOLLIN || event == EPOLLOUT)
	_event = event;
	else
		throw std::invalid_argument("Only use EPOLLIN or EPOLLOUT");
}

void TCPStream::setMessage(const std::string &message)
{
	_message = message;
}


int TCPStream::getFd() const
{
	return (_fd);
}
IPAddress TCPStream::getData() const
{
	return (_data);
}

uint32_t TCPStream::getEvent() const
{
	return (_event);
}

std::string TCPStream::getMessage() const
{
	return (_message);
}

std::ostream &operator<<(std::ostream &out, const TCPStream &stream)
{
	out << "Client fd: " << stream.getFd() << ", Event: ";
	if (stream.getEvent() == EPOLLIN)
		out << " EPOLLIN ";
	if (stream.getEvent() == EPOLLOUT)
		out << " EPOLLOUT ";
	out << ", Message: " << stream.getMessage();
//	out << ", Data: \n- Port: " << (uint16_t)ntohs((stream.getData()).sin_port);
//	out << "\n- Address: " << inet_ntoa(stream.getData().sin_addr) << '\n';
	return (out);
}
