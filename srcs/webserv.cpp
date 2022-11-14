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

int	main(int ac, char* av[])
{
	HTTPServer	http;

  if (ac == 2)
  {
    try
    {
      struct sigaction  sa;
      sa.sa_handler = &sigint_handler;
      sigemptyset(&sa.sa_mask);
      sa.sa_flags = 0;
      if (sigaction(SIGINT, &sa, NULL) != 0)
        throw ws_syscall_error("sigaction()");

      http.config().parseConfigFile(av[1]);
      http.start();
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
  }

	return 0;
}
