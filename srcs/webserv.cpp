/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiju-du <abiju-du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 10:39:57 by savosaicic        #+#    #+#             */
/*   Updated: 2022/11/03 16:22:32 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/epoll.h>
#include <webserv.hpp>
#include <TCPListener.hpp>
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
      std::cout << "message receive: " << stream.getInMessage() << std::endl;
      // listener.unRegisterClientEvent(stream, EPOLLIN);
      listener.registerClientEvent(stream, EPOLLOUT);
    }
  }
  else if (stream.triggeredEvents() & EPOLLOUT)
  {
    //stream.write(stream.getInMessage());
    stream.write("hello world");
    std::cout << stream.getInMessage() << "drop client\n";
    listener.dropClient(stream);
  }
}

int	main(void)
{
	TCPListener	tcp;

	std::vector<TCPStreamManager::iterator> incoming_streams;

	tcp.bind("0.0.0.0", "8080");
	tcp.listen();
	while (1)
  {
		incoming_streams = tcp.incoming();
    for (std::vector<TCPStreamManager::iterator>::iterator stream = incoming_streams.begin();
        stream != incoming_streams.end(); ++stream)
    {
      handle_connection(tcp, *stream->base());
    }
  }

	return (0);
}
