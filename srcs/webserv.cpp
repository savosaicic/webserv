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

//	request.parseRequest("GE");
//	request.parseRequest("T ");
////	request.parseRequest("POST");
//	request.parseRequest(" ");
//	request.parseRequest(" /");
//	request.parseRequest("ur");
//	request.parseRequest("i ");
//	request.parseRequest("HT");
//	request.parseRequest("TP");
//	request.parseRequest("/1");
//	request.parseRequest(".1\r\n");
//	request.parseRequest("Lang:");
//	request.parseRequest(" FR");
//	request.parseRequest(" \r");
//	request.parseRequest("\n");
//	request.parseRequest("Content-ty");
//	request.parseRequest("pe: html\r\n");
//	request.parseRequest("\r\n");
//	//request.parseRequeer("\r\n");
//	request.parseRequest("LORAINE IPSUME BLABLA LATIN");
//	request.parseRequest("i\r\n");
//	request.parseRequest("d\r\n");
//	request.parseRequest("\r\n");
//	request.parseRequest("OUT OF BODY");

//	request.parseRequest("GET /uri HTTP/1.1 \r\nLang:FR\r\nContent-type: html\r\n\r\n");
	request.parseRequest("POST /uri HTTP/1.1 \r\nLang:FR\r\nContent-type: html\r\nContent-Length: 29\r\n\r\nBONJOUR A TOUSSE LOREM IP SUM");
//	request.parseRequest("\r\n");
//	request.parseRequest("\r\n");
	

	std::cout << request;
}
