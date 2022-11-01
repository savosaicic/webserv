/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiju-du <abiju-du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 10:39:57 by savosaicic        #+#    #+#             */
/*   Updated: 2022/11/01 09:42:06 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <webserv.hpp>
#include <TCPListener.hpp>

#include "Config.hpp"

int	main(void) 
{
//	// HTTPServer	server;
//
//	// server.start();
//
//	TCPListener	tcp;
//
//	tcp.bind("0.0.0.0", "8080");
//	tcp.listen();
//	while (1)
//		tcp.incoming();

  Config  conf;

  conf.parseConfigFile("Makefile");

	return (0);
}
