#include <iostream>
#include <sys/epoll.h>
#include <webserv.hpp>
#include <TCPListener.hpp>
#include <HTTPRequest.hpp>
#include "TCPStreamManager.hpp"
#include "CGIRequest.hpp"
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

int	main(int ac, char **av, char **envp)
{
	if (ac && av)
	{
			HTTPRequest raw("GET /cgi/hello.cgi?key1=value1&key2=value2 HTTP/1.1 \r\nAccept-Language: \nIf-Modified-Since: Fri, 10 Dec 2004 11:22:13 GMT\n\nBonjour a tousse");

			CGIRequest cgi(raw, "cgi/", CGIRequest::PERL);
			char **new_envp = cgi.prepareCGI(envp);
			cgi.executeCGI(new_envp);
	}

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
	request.parseRequestHeader("\r\n");
	request.parseRequestHeader("LORAINE IPSUME BLABLA LATIN");
	request.parseRequestHeader("i\r\n");
	request.parseRequestHeader("d\r\n");
	request.parseRequestHeader("\r\n");
	request.parseRequestHeader("OUT OF BODY");

	std::cout << request;
}
