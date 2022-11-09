#ifndef HTTP_STREAM_HPP
#define HTTP_STREAM_HPP

#include "VirtualServer.hpp"
#include <TCPStream.hpp>
#include <HTTPRequest.hpp>
#include <string>

class HTTPStream
{
  private:
    TCPStream     _tcp;
    HTTPRequest   _request;
    VirtualServer _srv_config;

  public:
    HTTPStream();
    HTTPStream(const HTTPStream& src);
    HTTPStream(const TCPStream& tcp);
    ~HTTPStream();

    HTTPStream& operator=(const HTTPStream& rhs);

    const TCPStream&  tcp() const;
    const HTTPRequest&  getRequest() const;
    const VirtualServer&  getVirtualServer() const;
    
    void  setServerConfig(const VirtualServer& srv_conf);
    void  setResponse(const std::string& str);
};

#endif
