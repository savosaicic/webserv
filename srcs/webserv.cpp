/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiju-du <abiju-du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 10:39:57 by savosaicic        #+#    #+#             */
/*   Updated: 2022/10/28 16:37:58 by abiju-du         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <webserv.hpp>
#include <TCPListener.hpp>

int	main(void) 
{
	// HTTPServer	server;

	// server.start();

	TCPListener	tcp;
	tcp.bind("0.0.0.0", "8080");
	// tcp.listen();

	tcp.bind("0.0.0.0", "9080");
	tcp.bind("0.0.0.0", "9081");
	tcp.bind("0.0.0.0", "9082");
	tcp.bind("0.0.0.0", "9083");
	// tcp.listen();

	tcp.bind("0.0.0.0", "9084");
	tcp.bind("0.0.0.0", "9085");
	tcp.listen();
	while (1)
	{
		tcp.incoming();
	}
		
	return (0);
}
