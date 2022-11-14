#include "VirtualServer.hpp"

VirtualServer::Redirection::Redirection() : _location(),
  _status_code(0), _on(false)
{}

VirtualServer::Redirection::Redirection(const Redirection& src)
  : _location(src._location), _status_code(src._status_code), _on(src._on)
{}

VirtualServer::Redirection::Redirection(const std::string& location,
    const int code) : _location(location), _status_code(code), _on(true)
{}

VirtualServer::Redirection::~Redirection()
{}

uint16_t  VirtualServer::Redirection::statusCode() const
{
  return _status_code;
}

const std::string& VirtualServer::Redirection::getLocation() const
{
  return _location;
}

void  VirtualServer::Redirection::setLocation(const std::string& location)
{
  _location = location;
}

void  VirtualServer::Redirection::setStatusCode(const uint16_t code)
{
  _status_code = code;
}

VirtualServer::Redirection&
VirtualServer::Redirection::operator=(const Redirection& rhs)
{
  if (this != &rhs)
  {
    _location = rhs._location;
    _status_code = rhs._status_code;
    _on = rhs._on;
  }
  return *this;
}
