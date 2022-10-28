/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPStream.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <lefendie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:58:26 by lefendie          #+#    #+#             */
/*   Updated: 2022/10/28 11:27:54 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <TCPStream.hpp>

#include <arpa/inet.h>

TCPStream::TCPStream(int fd, sockaddr_in data): _fd(fd), _data(data)
{
	
}

TCPStream::~TCPStream()
{

}

TCPStream &TCPStream::operator=(const TCPStream &rhs)
{
	_fd = rhs._fd;
	_data = rhs._data;

	return (*this);
}

TCPStream::TCPStream(const TCPStream &src)
{
	_fd = src._fd;
	_data = src._data;
}
