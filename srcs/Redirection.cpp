/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Redirection.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasaicic <sasaicic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:01:25 by sasaicic          #+#    #+#             */
/*   Updated: 2022/10/28 17:20:05 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
