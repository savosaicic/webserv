#include <HTTPStream.hpp>
#include <string>

HTTPStream::HTTPStream()
{}

HTTPStream::HTTPStream(const HTTPStream& src)
  : _tcp(src._tcp), _request(src._request), _srv_config(src._srv_config)
{}

HTTPStream::HTTPStream(const TCPStream& tcp)
  : _tcp(tcp)
{
  //Set the _request to empty
}

HTTPStream::~HTTPStream()
{}

HTTPStream& HTTPStream::operator=(const HTTPStream& rhs)
{
  if (this != &rhs)
  {
    _tcp = rhs._tcp;
    _request = rhs._request;
    _srv_config = rhs._srv_config;
  }
  return *this;
}

const TCPStream&  HTTPStream::tcp() const
{
  return _tcp;
}

const HTTPRequest&  HTTPStream::getRequest() const
{
  return _request;
}

void  HTTPStream::setServerConfig(const VirtualServer& srv_conf)
{
  _srv_config = srv_conf;
}

void  HTTPStream::setResponse(const std::string& str)
{
  _tcp.setOutMessage(str);
}
