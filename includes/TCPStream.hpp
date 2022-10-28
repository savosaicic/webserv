/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPStream.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <lefendie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:58:44 by lefendie          #+#    #+#             */
/*   Updated: 2022/10/28 11:51:36 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSTREAM_H
# define TCPSTREAM_H

#include <arpa/inet.h>
#include <string>

class TCPStream
{
	private:
		int _fd;
		sockaddr_in _data;
		int _event;
		std::string _message;
	
	public:
		TCPStream(int fd, sockaddr_in data);
		~TCPStream();
		TCPStream &operator=(const TCPStream &rhs);
		TCPStream(const TCPStream &src);
		
		void setEvent(int event);
		void setMessage(std::string message);

};

#endif
