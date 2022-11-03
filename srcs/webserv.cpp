/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiju-du <abiju-du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 10:39:57 by savosaicic        #+#    #+#             */
/*   Updated: 2022/11/03 08:26:48 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/epoll.h>
#include <webserv.hpp>
#include <TCPListener.hpp>
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
	typedef std::vector<std::vector<TCPStream>::iterator>::iterator stream_iterator;

	TCPListener	tcp;

	std::vector<std::vector<TCPStream>::iterator> stream_vect;

	tcp.bind("0.0.0.0", "8080");
	tcp.listen();
	while (1)
  {
		stream_vect = tcp.incoming();
    for (stream_iterator stream = stream_vect.begin();
        stream != stream_vect.end(); ++stream)
    {
      handle_connection(tcp, *stream->base());
    }
  }

	return (0);
}
