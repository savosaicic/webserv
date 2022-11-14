#include <HTTPVersion.hpp>
#include <string>

const std::string& HTTPVersion::_prefix = "HTTP/";

HTTPVersion::HTTPVersion() : _version(0, 0)
{}

HTTPVersion::HTTPVersion(const HTTPVersion& src) : _version(src._version)
{}

HTTPVersion::HTTPVersion(const std::pair<uint16_t, uint16_t>& version)
  : _version(version)
{}

HTTPVersion::HTTPVersion(const uint16_t major, const uint16_t minor)
  : _version(major, minor)
{}

HTTPVersion::~HTTPVersion()
{}

HTTPVersion&  HTTPVersion::operator=(const HTTPVersion& rhs)
{
  if (this != &rhs)
    _version = rhs._version;
  return *this;
}

bool  HTTPVersion::isSet() const
{
  return _version.first != 0 || _version.second != 0;
}

const std::string&  HTTPVersion::getPrefix() const
{
  return _prefix;
}

uint16_t  HTTPVersion::getMajor() const
{
  return _version.first;
}

uint16_t  HTTPVersion::getMinor() const
{
  return _version.second;
}

void  HTTPVersion::setMajor(uint16_t major)
{
  _version.first = major;
}

void  HTTPVersion::setMinor(uint16_t minor)
{
  _version.second = minor;
}

bool  HTTPVersion::operator==(const HTTPVersion& rhs)
{
  return _version.first == rhs._version.first &&
    _version.second == rhs._version.second;
}

bool  HTTPVersion::operator!=(const HTTPVersion& rhs)
{
  return !(*this == rhs);
}

bool  HTTPVersion::operator<(const HTTPVersion& rhs)
{
  return _version.first < rhs._version.first ||
    _version.second < rhs._version.second;
}

bool  HTTPVersion::operator>(const HTTPVersion& rhs)
{
  return _version.first > rhs._version.first ||
    _version.second > rhs._version.second;
}

bool  HTTPVersion::operator<=(const HTTPVersion& rhs)
{
  return *this < rhs || *this == rhs;
}

bool  HTTPVersion::operator>=(const HTTPVersion& rhs)
{
  return *this > rhs || *this == rhs;
}
