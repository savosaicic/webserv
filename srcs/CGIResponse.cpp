#include "HTTPVersion.hpp"
#include <CGIResponse.hpp>
#include <http_utils.hpp>
#include <sstream>
#include <utils.hpp>

CGIResponse::CGIResponse() {}

CGIResponse::CGIResponse(const CGIResponse& src)
  : _version(src._version), _status(src._status), _cgi_output(src._cgi_output)
{}

CGIResponse::CGIResponse(const std::string& cgi_output,
    const HTTPVersion& version)
  : _version(version), _status(status_code::http_ok), _cgi_output(cgi_output)
{
}

CGIResponse::~CGIResponse() {}

CGIResponse& CGIResponse::operator=(const CGIResponse& rhs)
{
  if (this != &rhs)
  {
    _version = rhs._version;
    _status = rhs._status;
    _reason = rhs._reason;
    _cgi_output = rhs._cgi_output;
  }
  return *this;
}

void  CGIResponse::setReason(const std::string& reason)
{
  _reason = reason;
}

const HTTPVersion& CGIResponse::getVersion() const
{
  return _version;
}

int CGIResponse::getStatusCode() const
{
  return _status;
}

const std::string&  CGIResponse::getReason() const
{
  return _reason;
}

const std::string& CGIResponse::raw()
{
  std::stringstream ss;

  _raw.clear();
  ss << "HTTP/" << _version.getMajor() << "." << _version.getMinor() <<
    SP << _status << SP << "TODO" << CRLF;
  ss << http_headers::connection << ": close" << CRLF;
  ss << _cgi_output;
  _raw = ss.str();
  return _raw;
}

int CGIResponse::size() const
{
  return _raw.size();
}
