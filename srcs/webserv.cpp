#include <csignal>
#include <exception>
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
#include <signal.h>
#include <ws_syscall_error.hpp>

void  sigint_handler(int signal)
{
  if (signal == SIGINT)
    listening = false;
}

int	main(int argument_count, char **argument_value, char **environement_parameters)
{
	HTTPServer	http;

  try
  {
    struct sigaction  sa;
    sa.sa_handler = &sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) != 0)
      throw ws_syscall_error("sigaction()");

    http.config().parseConfigFile("Makefile");
    http.start();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

	return 0;
}
