#include "VirtualServer.hpp"
#include <algorithm>

VirtualServer::Location::Location() : _redirection_set(false)
{
}

VirtualServer::Location::Location(const Location& src)
  : _location(src._location), _docroot(src._docroot),
  _autoindex(src._autoindex), _redirection(src._redirection),
  _redirection_set(src._redirection_set), _index_list(src._index_list),
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
    _redirection = rhs._redirection;
    _redirection_set = rhs._redirection_set;
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

void  VirtualServer::Location::setRedirection(
    const VirtualServer::Redirection& redirection)
{
  _redirection = redirection;
  _redirection_set = true;
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

bool  VirtualServer::Location::isRedirectionSet() const
{
  return _redirection_set;
}

const VirtualServer::Redirection&
VirtualServer::Location::getRedirection() const
{
  return _redirection;
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

VirtualServer::Location::index_list_t::iterator
VirtualServer::Location::indexBegin()
{ return _index_list.begin(); }

VirtualServer::Location::index_list_t::iterator
VirtualServer::Location::indexEnd()
{ return _index_list.end(); }

VirtualServer::Location::index_list_t::const_iterator
VirtualServer::Location::indexBegin() const
{ return _index_list.begin(); }

VirtualServer::Location::index_list_t::const_iterator
VirtualServer::Location::indexEnd() const
{ return _index_list.end(); }
