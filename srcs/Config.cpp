/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasaicic <sasaicic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 16:31:11 by sasaicic          #+#    #+#             */
/*   Updated: 2022/11/01 09:17:12 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <ostream>
#include <stdexcept>
#include <string>


Config::Config()
{}

Config::Config(const Config& src) : _server_list(src._server_list)
{}

Config::~Config()
{}

Config& Config::operator=(const Config& rhs)
{
  if (this != &rhs)
  {
    _server_list = rhs._server_list;
  }
  return *this;
}

void  Config::parseConfigFile(const char* filename)
{
  std::ifstream file(filename);
  std::string   line;

  if (!file.is_open())
  {
    std::cerr << "Can't open file: " << filename
      << std::endl << std::strerror(errno) << std::endl;
    throw std::ifstream::failure("");
  }

  while (std::getline(file, line))
  {
    std::cout << line << std::endl;
  }
  std::cout << "........................" << std::endl;

  VirtualServer server;
  server.addErrorPage(404, "404.html");
  server.setListeningHost("hostname");
  server.setListeningPort("9090");

  _server_list.push_back(server);
}
