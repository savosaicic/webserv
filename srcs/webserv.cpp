/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiju-du <abiju-du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 10:39:57 by savosaicic        #+#    #+#             */
/*   Updated: 2022/11/07 09:32:22 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <webserv.hpp>
#include <TCPListener.hpp>
#include <HTTPRequest.hpp>
#include <vector>

#include "Config.hpp"

int	main(void) 
{
//	TCPListener	tcp;
//
//	std::vector<TCPStream> stream_vect;
//
//	tcp.bind("0.0.0.0", "8080");
//	tcp.listen();
//	while (1)
//		stream_vect = tcp.incoming();
//

	HTTPRequest raw("GET /software/htp/cics/index.html HTTP/1.1\nAccept-Language: fr, de\nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\n\nBonjour a tousse");

	std::cout << raw;

	return (0);
}
