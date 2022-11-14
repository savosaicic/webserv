#include "VirtualServer.hpp"
#include "default_conf_value.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <utility>

std::pair<unsigned short, std::string> VirtualServer::map_data[5] = {
  std::make_pair(400, VirtualServer::_createErrorPage(400, "Forbidden")),
  std::make_pair(403, VirtualServer::_createErrorPage(403, "Forbidden")),
  std::make_pair(404, VirtualServer::_createErrorPage(404, "Not Found")),
  std::make_pair(405, VirtualServer::_createErrorPage(405, "Method Not Allowed")),
  std::make_pair(500, VirtualServer::_createErrorPage(500, "Internal Server Error"))
};

const std::map<unsigned short, std::string>
VirtualServer::_default_error_page_list(VirtualServer::map_data,
    VirtualServer::map_data + sizeof VirtualServer::map_data / sizeof VirtualServer::map_data[0]);

VirtualServer::VirtualServer() : _redirection_set(false),
  _client_max_body_size(DEFAULT_CLIENT_BODY_SIZE)
{
}

VirtualServer::VirtualServer(const VirtualServer& src)
  : _host(src._host),
  _port(src._port),
  _redirection(src._redirection), _redirection_set(src._redirection_set),
  _error_page_file_list(src._error_page_file_list),
  _client_max_body_size(src._client_max_body_size),
  _cgi_list(src._cgi_list),
  _server_name_list(src._server_name_list),
  _location_list(src._location_list)
{}

VirtualServer::~VirtualServer()
{
}

VirtualServer&  VirtualServer::operator=(const VirtualServer& rhs)
{
  if (this != &rhs)
  {
    _host = rhs._host;
    _port = rhs._port;
    _redirection = rhs._redirection;
    _redirection_set = rhs._redirection_set;
    _error_page_file_list = rhs._error_page_file_list;
    _client_max_body_size = rhs._client_max_body_size;
    _cgi_list = rhs._cgi_list;
    _server_name_list = rhs._server_name_list;
    _location_list = rhs._location_list;
  }
  return *this;
}

const std::string&  VirtualServer::getListeningHost() const
{
  return _host;
}

const std::string&  VirtualServer::getListeningPort() const
{
  return _port;
}

bool  VirtualServer::isRedirectionSet() const
{
  return _redirection_set;
}

const VirtualServer::Redirection&  VirtualServer::getRedirection() const
{
  return _redirection;
}

const std::string VirtualServer::getErrorPage(const uint16_t error_num) const
{
  std::map<uint16_t, std::string>::const_iterator it;

  it = _error_page_file_list.find(error_num);
  if (it != _error_page_file_list.end())
  {
    std::ifstream filestream(it->second.c_str());
    if (filestream.is_open() == false)
      goto default_error_page;
    std::stringstream buffer;
    buffer << filestream.rdbuf();
    return buffer.str();
  }

default_error_page:
  it = _default_error_page_list.find(error_num);
  if (it == _default_error_page_list.end())
    throw std::logic_error("Default error page not found");
  return it->second;
}

uint64_t VirtualServer::getClientMaxBodySize() const
{
  return _client_max_body_size;
}

const VirtualServer::Location&
VirtualServer::getLocation(const std::string& uri_src) const
{
  std::string uri = uri_src;
  if (uri.size() > 1 && *uri.rbegin() == '/')
    uri.erase(--(uri.rbegin().base()));

  std::map<std::string, Location>::const_iterator
    location(_location_list.begin());
	if (location == _location_list.end())
    throw std::runtime_error("");

  for (; !uri.empty(); ++location)
  {
    if (location == _location_list.end())
    {
      uri = uri.substr(0, uri.find_last_of('/'));
      if (uri.empty())
        return _location_list.find("/")->second;
      location = _location_list.begin();
    }
    if (location->second.getLocationPath() == uri)
      break;
  }
  return location->second;
}

bool  VirtualServer::isServerNameSet(const std::string& name) const
{
  return std::find(_server_name_list.begin(), _server_name_list.end(), name)
    != _server_name_list.end();
}

void  VirtualServer::setListeningHost(const std::string& host)
{
  _host = host;
}

void  VirtualServer::setListeningPort(const std::string& port)
{
  _port = port;
}

void  VirtualServer::setRedirection(
    const VirtualServer::Redirection& redirection)
{
  _redirection = redirection;
  _redirection_set = true;
}

void  VirtualServer::addLocation(const Location& location)
{
  _location_list.insert(std::make_pair(location.getLocationPath(), location));
}

void  VirtualServer::addServerName(const std::string& name)
{
  _server_name_list.push_back(name);
}

void  VirtualServer::addErrorPage(const uint16_t error_num,
    const std::string& filename)
{
  _error_page_file_list[error_num] = filename;
}

void  VirtualServer::setClientMaxBodySize(const uint64_t size)
{
  _client_max_body_size = size;
}

const std::string
VirtualServer::_createErrorPage(const uint16_t code, const std::string& msg)
{
  std::ostringstream  ss;

  ss << "<html><head><title>" << code << " " << msg
    << "</title></head><body><center><h1>" << code
    << " " << msg <<"</h1></center></body></html>";
  return ss.str();
}
