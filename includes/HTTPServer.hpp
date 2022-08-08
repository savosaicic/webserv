/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasaicic <sasaicic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 10:47:02 by sasaicic          #+#    #+#             */
/*   Updated: 2022/08/08 14:10:38 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_HPP
# define HTTP_SERVER_HPP

#include <TCPServer.hpp>
#include <netinet/in.h>

class	HTTPServer : public TCPServer
{
	private:
		int					_port;
		struct sockaddr_in	_serveraddr;

	public:
		HTTPServer(/* args */);
		~HTTPServer();
		
		void	start();
};

#endif
