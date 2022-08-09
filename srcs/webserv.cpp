#include <exception>
#include <iostream>
#include <ostream>
#include "HTTPServer.hpp"
#include "utils.hpp"

int main(void)
{
	HTTPServer	server;

	try
	{
		server.start();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return (0);
}
