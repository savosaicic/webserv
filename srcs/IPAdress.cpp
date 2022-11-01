/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPAdress.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasaicic <sasaicic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 15:16:57 by sasaicic          #+#    #+#             */
/*   Updated: 2022/11/01 17:01:04 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IPAddress.hpp"
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>

#include <stdio.h> // printf

IPAddress::IPAddress() : _defined(false)
{
}

IPAddress::IPAddress(const IPAddress& src) : _addr(src._addr),
  _defined(src._defined)
{
}

IPAddress::IPAddress(const struct sockaddr addr, socklen_t addrlen)
  : _addr(addr), _addrlen(addrlen), _defined(true)
{
}

IPAddress::~IPAddress()
{
}

IPAddress&  IPAddress::operator=(const IPAddress& rhs)
{
  if (this != &rhs)
  {
    _addr = rhs._addr;
    _addrlen = rhs._addrlen;
    _defined = rhs._defined;
  }
  return *this;
}

const std::string IPAddress::toStr() const
{
  char host[512];
  char service[32];

  memset(&host, 0, sizeof(host));
  memset(&service, 0, sizeof(service));

  int ret = getnameinfo(&_addr, _addrlen, &host[0],
      sizeof(host), &service[0], sizeof(service), 0);
  if (ret != 0)
  {
    printf("error %s\n", gai_strerror(ret));
    throw std::runtime_error("getnameinfo() failed");
  }
  return std::string(host); 
}

const sockaddr    IPAddress::addr() const
{
  return _addr;
}

socklen_t    IPAddress::addrlen() const
{
  return _addrlen;
}
