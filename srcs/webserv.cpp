#include <iostream>
#include <sys/epoll.h>
#include <TCPListener.hpp>
#include <HTTPRequest.hpp>
#include "TCPStreamManager.hpp"
#include <vector>
#include "Config.hpp"
#include <HTTPServer.hpp>

int	main(void)
{
	HTTPServer	http;

  try
  {
    http.config().parseConfigFile("Makefile");
    http.start();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

	return 0;
}
