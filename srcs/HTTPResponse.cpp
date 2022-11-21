#include "HTTPVersion.hpp"
#include <HTTPResponse.hpp>
#include <sstream>
#include <utils.hpp>

HTTPResponse::HTTPResponse() {}

HTTPResponse::HTTPResponse(const HTTPResponse& src)
  : _version(src._version), _status(src._status),
  _headers(src._headers), _body(src._body) {}

HTTPResponse::HTTPResponse(const VirtualServer::Redirection& redirection,
    const HTTPVersion& version)
  : _version(version), _status(redirection.statusCode())
{
  _headers[http_headers::location] = redirection.getLocation();
}

HTTPResponse::HTTPResponse(const int status, const std::string& body,
    const std::string& content_type, const HTTPVersion& version)
  : _version(version), _status(status), _body(body)
{
  if (!body.empty())
  {
    _headers[http_headers::content_type] = content_type;
    _headers[http_headers::content_length] = to_string(_body.size());
  }
}

HTTPResponse::~HTTPResponse() {}

HTTPResponse& HTTPResponse::operator=(const HTTPResponse& rhs)
{
  if (this != &rhs)
  {
    _version = rhs._version;
    _status = rhs._status;
    _reason = rhs._reason;
    _headers = rhs._headers;
    _body = rhs._body;
  }
  return *this;
}

void  HTTPResponse::setVersion(const HTTPVersion& version)
{
  _version = version;
}

void  HTTPResponse::setStatus(const int status)
{
  _status = status;
}

void  HTTPResponse::setReason(const std::string& reason)
{
  _reason = reason;
}

void  HTTPResponse::setHeaders(const std::map<std::string, std::string>& headers)
{
  _headers = headers;
}

void  HTTPResponse::setBody(const std::string& body)
{
  _body = body;
}

void  HTTPResponse::addHeader(const std::string& name, const std::string& value)
{
  _headers[name] = value;
}

const HTTPVersion& HTTPResponse::getVersion() const
{
  return _version;
}

int HTTPResponse::getStatusCode() const
{
  return _status;
}

const std::string&  HTTPResponse::getReason() const
{
  return _reason;
}

const std::map<std::string, std::string>& HTTPResponse::getHeaders() const
{
  return _headers;
}

const std::string&  HTTPResponse::getBody() const
{
  return _body;
}

const std::string& HTTPResponse::raw()
{
  std::stringstream ss;

  _raw.clear();
  ss << "HTTP/" << _version.getMajor() << "." << _version.getMinor() <<
    SP << _status << SP << "TODO" << CRLF;
  for (std::map<std::string, std::string>::const_iterator it =
      _headers.begin(); it != _headers.end(); ++it)
  {
    ss << it->first << ": " << it->second << CRLF;
  }
  ss << CRLF;
  ss << _body;
  _raw = ss.str();
  return _raw;
}

int HTTPResponse::size() const
{
  return _raw.size();
}
