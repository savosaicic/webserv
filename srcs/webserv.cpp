/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiju-du <abiju-du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 10:39:57 by savosaicic        #+#    #+#             */
/*   Updated: 2022/11/08 09:54:13 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/epoll.h>
#include <webserv.hpp>
#include <TCPListener.hpp>
#include <HTTPRequest.hpp>
#include "TCPStreamManager.hpp"
#include <vector>

#include "Config.hpp"

void  handle_connection(TCPListener& listener, TCPStream& stream)
{
  if (stream.triggeredEvents() & EPOLLIN)
  {
    if (stream.read() == 0)
    {
      std::cout << "disconnected client" << std::endl;
      listener.dropClient(stream);
    }
    else
    {
      std::cout << "recived from: " << stream.getIPAddr().toStr()
				<< ": " << std::endl;
      std::cout << stream.getInMessage() << std::endl << std::endl;
      listener.registerClientEvent(stream, EPOLLOUT);
    }
  }
  else if (stream.triggeredEvents() & EPOLLOUT)
  {
    stream.write("HTTP/1.1 200 Ok\r\n\r\n");
    listener.dropClient(stream);
  }
}

int	main(void)
{
//	TCPListener	tcp;
//
//	std::vector<TCPStreamManager::iterator> incoming_streams;
//
//	tcp.bind("0.0.0.0", "8080");
//	tcp.listen();
//	while (1)
//  {
//		incoming_streams = tcp.incoming();
//    for (std::vector<TCPStreamManager::iterator>::iterator stream =
//        incoming_streams.begin(); stream != incoming_streams.end(); ++stream)
//    {
//      handle_connection(tcp, (*stream)->second);
//    }
//  }
	HTTPRequest raw("GET /software/htp/cics/index.html HTTP/1.2\nAccept-Language: fr, de\nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\n\nBonjour a tousse");

	std::cout << raw;

	std::cout << raw.getHeaderContent("Accept-Language") << std::endl;
}
