/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPStream.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <lefendie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:58:44 by lefendie          #+#    #+#             */
/*   Updated: 2022/10/28 11:31:26 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSTREAM_H
# define TCPSTREAM_H

#include <arpa/inet.h>

class TCPStream
{
	private:
		int _fd;
		sockaddr_in _data;
	
	public:
		TCPStream(int fd, sockaddr_in data);
		~TCPStream();
		TCPStream &operator=(const TCPStream &rhs);
		TCPStream(const TCPStream &src);

};

#endif
