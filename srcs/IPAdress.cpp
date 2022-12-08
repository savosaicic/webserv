#include "IPAddress.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>

#include <stdio.h> // printf

IPAddress::IPAddress() : _defined(false)
{
}

IPAddress::IPAddress(const IPAddress& src) : _ip_str(src._ip_str),
  _port_str(src._port_str), _addr(src._addr), _addrlen(src._addrlen),
  _defined(src._defined)
{
}

IPAddress::IPAddress(const struct sockaddr addr, const socklen_t addrlen)
  : _addr(addr), _addrlen(addrlen), _defined(true)
{
  char host[NI_MAXHOST];
  char service[NI_MAXSERV];

  memset(&host, 0, sizeof(host));
  memset(&service, 0, sizeof(service));

  int ret = getnameinfo(&_addr, _addrlen, &host[0],
      sizeof(host), &service[0], sizeof(service), NI_NUMERICSERV);
  if (ret != 0)
  {
    printf("error %s\n", gai_strerror(ret));
    throw std::runtime_error("getnameinfo() failed");
  }
  _ip_str = host;
  _port_str = service;
}

IPAddress::~IPAddress()
{
}

IPAddress&  IPAddress::operator=(const IPAddress& rhs)
{
  if (this != &rhs)
  {
    _ip_str = rhs._ip_str;
    _port_str = rhs._port_str;
    _addr = rhs._addr;
    _addrlen = rhs._addrlen;
    _defined = rhs._defined;
  }
  return *this;
}

const std::string& IPAddress::port() const
{
  return _port_str;
}

const std::string& IPAddress::toStr() const
{
  return _ip_str;
}

const sockaddr    IPAddress::addr() const
{
  return _addr;
}

socklen_t    IPAddress::addrlen() const
{
  return _addrlen;
}

bool  IPAddress::operator==(const IPAddress& rhs)
{
  return _ip_str == rhs._ip_str && _port_str == rhs._port_str;
}
