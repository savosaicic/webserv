#include <csignal>
#include <exception>
#include <iostream>
#include <sys/epoll.h>
#include <TCPListener.hpp>
#include <HTTPRequest.hpp>
#include "TCPStreamManager.hpp"
#include <vector>
#include "Config.hpp"
#include <HTTPServer.hpp>
#include <signal.h>
#include <ws_syscall_error.hpp>

void  sigint_handler(int signal)
{
  if (signal == SIGINT)
    listening = false;
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
	request.parseRequestHeader("\r\n");
	request.parseRequestHeader("LORAINE IPSUME BLABLA LATIN");
	request.parseRequestHeader("i\r\n");
	request.parseRequestHeader("d\r\n");
	request.parseRequestHeader("\r\n");
	request.parseRequestHeader("OUT OF BODY");

	std::cout << request;
}
