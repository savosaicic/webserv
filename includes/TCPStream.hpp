/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPStream.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <lefendie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:58:44 by lefendie          #+#    #+#             */
/*   Updated: 2022/10/28 18:30:07 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSTREAM_H
# define TCPSTREAM_H

#include <arpa/inet.h>
#include <string>
#include <iostream>

class TCPStream
{
	private:
		int _fd;
		sockaddr_in _data;
		int _event;
		std::string _message;
	
	public:
		TCPStream();
		TCPStream(int fd, sockaddr_in data, int event);
		~TCPStream();
		TCPStream &operator=(const TCPStream &rhs);
		TCPStream(const TCPStream &src);
		
		void setEvent(const int event);
		void setMessage(const std::string &message);

		int getFd() const;
		sockaddr_in getData() const;
		int getEvent() const;
		std::string getMessage() const;

};

std::ostream &operator<<(std::ostream &out, const TCPStream &stream);


#endif
