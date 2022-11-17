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
	HTTPRequest request;

	request.parseRequestLine("GE");
	request.parseRequestLine("T ");
	request.parseRequestLine(" /");
	request.parseRequestLine("ur");
	request.parseRequestLine("i ");
	request.parseRequestLine("HT");
	request.parseRequestLine("TP");
	request.parseRequestLine("/1");
	request.parseRequestLine(".1\r\n");
	request.parseRequestHeader("Lang:");
	request.parseRequestHeader(" FR");
	request.parseRequestHeader(" \r");
	request.parseRequestHeader("\n");
	request.parseRequestHeader("Content-ty");
	request.parseRequestHeader("pe: html\r\n");

	std::cout << request;
}
