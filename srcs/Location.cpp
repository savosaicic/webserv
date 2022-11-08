/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasaicic <sasaicic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 11:02:40 by sasaicic          #+#    #+#             */
/*   Updated: 2022/11/08 10:19:37 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtualServer.hpp"
#include <algorithm>

VirtualServer::Location::Location()
{
}

VirtualServer::Location::Location(const Location& src)
  : _location(src._location), _docroot(src._docroot),
  _autoindex(src._autoindex), _index_list(src._index_list),
  _allowed_method_list(src._allowed_method_list)
{}

VirtualServer::Location::~Location()
{
}

VirtualServer::Location& VirtualServer::Location::operator=(const Location& rhs)
{
  if (this != &rhs)
  {
    _location = rhs._location;
    _docroot = rhs._docroot;
    _autoindex = rhs._autoindex;
    _index_list = rhs._index_list;
    _allowed_method_list = rhs._allowed_method_list;
  }
  return *this;
}

void  VirtualServer::Location::setLocationPath(const std::string &location)
{
  _location = location;
}

void  VirtualServer::Location::setDocroot(const std::string& docroot)
{
  _docroot = docroot;
}

void  VirtualServer::Location::setAutoindex(const bool autoindex)
{
  _autoindex = autoindex;
}

void  VirtualServer::Location::addIndexFileToList(const std::string& index)
{
  if (!isIndexInList(index))
    _index_list.push_back(index);
}

void  VirtualServer::Location::allowMethod(const HTTPRequest::method_t method)
{
  if (!isMethodAllowed(method))
    _allowed_method_list.push_back(method);
}


const std::string& VirtualServer::Location::getLocationPath() const
{
  return _location;
}

const std::string& VirtualServer::Location::getDocroot() const
{
  return _docroot;
}

bool  VirtualServer::Location::getAutoindex() const
{
  return _autoindex;
}

bool  VirtualServer::Location::isIndexInList(const std::string& index) const
{
  if (std::find(_index_list.begin(),
        _index_list.end(), index) != _index_list.end())
    return true;
  return false;
}

bool  VirtualServer::Location::isMethodAllowed(const HTTPRequest::method_t method) const
{
  if (std::find(_allowed_method_list.begin(),
        _allowed_method_list.end(), method) != _allowed_method_list.end())
    return true;
  return false;
}
