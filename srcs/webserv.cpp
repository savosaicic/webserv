/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiju-du <abiju-du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 10:39:57 by savosaicic        #+#    #+#             */
/*   Updated: 2022/10/28 16:46:59 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <webserv.hpp>
#include <TCPListener.hpp>
#include <vector>

int	main(void) 
{
	// HTTPServer	server;

	// server.start();

	TCPListener	tcp;

	std::vector<TCPStream> stream_vect;

	tcp.bind("0.0.0.0", "8080");
	tcp.listen();
	while (1)
		stream_vect = tcp.incoming();

	return (0);
}
