/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPStream.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <lefendie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:58:44 by lefendie          #+#    #+#             */
/*   Updated: 2022/11/02 08:44:00 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSTREAM_H
# define TCPSTREAM_H

#include <arpa/inet.h>
#include <string>
#include <iostream>
#include "IPAddress.hpp"

class TCPStream
{
	private:
		int _fd;
		IPAddress _ip;
		uint32_t _events;
		uint32_t _triggered_events;
		std::string _message;
		int _port;
	
	public:
		TCPStream();
		TCPStream(const int fd, const uint32_t events);
		~TCPStream();
		TCPStream& operator=(const TCPStream& rhs);
		TCPStream(const TCPStream& src);
		
		void registerEvent(const uint32_t events);
		void resetEvents();
		void setMessage(const std::string &message);

		int getFd() const;
		const IPAddress& getIPAddr() const;
		uint32_t triggeredEvents() const;
		const std::string& getMessage() const;

};

std::ostream &operator<<(std::ostream &out, const TCPStream &stream);


#endif
