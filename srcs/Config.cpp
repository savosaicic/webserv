#include "Config.hpp"
#include "VirtualServer.hpp"
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <ostream>
#include <stdexcept>
#include <string>


Config::Config() : _defined(false)
{}

Config::Config(const Config& src) : _defined(src._defined),
  _server_list(src._server_list)
{}

Config::~Config()
{}

Config& Config::operator=(const Config& rhs)
{
  if (this != &rhs)
  {
    _defined = rhs._defined;
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
    //std::cout << line << std::endl;
  }
  std::cout << "Config file reading done..." << std::endl;

  VirtualServer server;
  VirtualServer::Location location;

  location.setLocationPath("/");
  location.setDocroot("/home/path");
  location.setAutoindex(false);
  location.addIndexFileToList("index.html");
  location.allowMethod(HTTPRequest::GET);

  server.addErrorPage(404, "404.html");
  server.setListeningHost("0.0.0.0");
  server.setListeningPort("9090");
  server.addServerName("0.0.0.0");
  server.addLocation(location);

  _server_list.push_back(server);

  _defined = true;
}

const VirtualServer&  Config::getServer(const std::string& listening_addr,
    const std::string& port, const std::string& server_name) const
{
  for (std::vector<VirtualServer>::const_iterator it = _server_list.begin();
      it != _server_list.end(); ++it)
  {
    if (it->getListeningHost() == listening_addr &&
        it->getListeningPort() == port && it->isServerNameSet(server_name))
      return *it;
  }
  throw std::runtime_error("");
}

const std::vector<VirtualServer>& Config::getServerList()
{
  return _server_list;
}

bool  Config::isDefined()
{
  return _defined;
}
