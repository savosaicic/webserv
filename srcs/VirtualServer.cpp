#include "VirtualServer.hpp"
#include "default_conf_value.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <string>

VirtualServer::VirtualServer() : _client_max_body_size(DEFAULT_CLIENT_BODY_SIZE)
{
  _default_error_page_list[400] = _createErrorPage(400, "Bad Request");
  _default_error_page_list[403] = _createErrorPage(403, "Forbidden");
  _default_error_page_list[404] = _createErrorPage(404, "Not Found");
  _default_error_page_list[405] = _createErrorPage(405, "Method Not Allowed");
  _default_error_page_list[500] = _createErrorPage(500, "Internal Server Error");
}

VirtualServer::VirtualServer(const VirtualServer& src)
  : _host(src._host),
  _port(src._port),
  _default_error_page_list(src._default_error_page_list),
  _error_page_file_list(src._error_page_file_list),
  _client_max_body_size(src._client_max_body_size),
  _cgi_list(src._cgi_list),
  _server_name_list(src._server_name_list),
  _location_list(src._location_list)
{}

VirtualServer::~VirtualServer()
{}

VirtualServer&  VirtualServer::operator=(const VirtualServer& rhs)
{
  if (this != &rhs)
  {
    _host = rhs._host;
    _port = rhs._port;
    _default_error_page_list = rhs._default_error_page_list;
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

const std::string VirtualServer::getErrorPage(const uint16_t error_num) const
{
  std::map<uint16_t, std::string>::const_iterator it;

  it = _error_page_file_list.find(error_num);
  if (it != _error_page_file_list.end())
  {
    std::ifstream filestream(it->second.c_str());
    std::stringstream buffer;
    buffer << filestream.rdbuf();
    return buffer.str();
  }

  it = _default_error_page_list.find(error_num);
  if (it == _default_error_page_list.end())
    return std::string(); // TODO: return 500 internal instead ?
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
  if (*uri.rbegin() == '/')
    uri.erase(uri.rbegin().base());

  std::vector<Location>::const_iterator location = _location_list.begin();
  for (; !uri.empty(); ++location)
  {
    if (location->getLocationPath() == uri)
      break;
    if (location == _location_list.end())
    {
      uri = uri.substr(0, uri.find_last_of('/'));
//    if (uri.empty())
//      return default location (/) if it exist
      location = _location_list.begin();
    }
  }
  return *location;
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
