#include <iostream>
#include <sys/epoll.h>
#include <TCPListener.hpp>
#include <HTTPRequest.hpp>
#include "TCPStreamManager.hpp"
#include "CGIRequest.hpp"
#include <vector>
#include "CGIRequest.hpp"
#include "Config.hpp"
#include <HTTPServer.hpp>

int	main(int argument_count, char **argument_value, char **environement_parameters)
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
