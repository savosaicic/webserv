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
		server.close();
		std::cerr << e.what() << std::endl;
	}

	server.close();


	return (0);
}
