/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPStream.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <lefendie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:58:44 by lefendie          #+#    #+#             */
/*   Updated: 2022/11/01 15:54:29 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSTREAM_H
# define TCPSTREAM_H

#include <arpa/inet.h>
#include <string>
#include <iostream>

class IPAddress{};

class TCPStream
{
	private:
		int _fd;
		IPAddress _data;
		uint32_t _event;
		std::string _message;
		int _port;
	
	public:
		TCPStream();
		TCPStream(int fd, IPAddress data, uint32_t event);
		~TCPStream();
		TCPStream &operator=(const TCPStream &rhs);
		TCPStream(const TCPStream &src);
		
		void setEvent(const uint32_t event);
		void setMessage(const std::string &message);

		int getFd() const;
		IPAddress getData() const;
		uint32_t getEvent() const;
		std::string getMessage() const;

};

std::ostream &operator<<(std::ostream &out, const TCPStream &stream);


#endif
