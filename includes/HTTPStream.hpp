#ifndef HTTP_STREAM_HPP
#define HTTP_STREAM_HPP

#include "VirtualServer.hpp"
#include <TCPStream.hpp>
#include <HTTPRequest.hpp>
#include <string>
#include <HTTPVersion.hpp>

class HTTPStream
{
  private:
    TCPStream     &_tcp;
  public:
    HTTPRequest   _request;
  private:
    VirtualServer _srv_config;
    VirtualServer::Location _location_config;

  public:
    HTTPStream(const HTTPStream& src);
    HTTPStream(TCPStream& tcp);
    ~HTTPStream();

    HTTPStream& operator=(const HTTPStream& rhs);

    TCPStream&  tcp();
    const HTTPRequest&  getRequest() const;
    const VirtualServer&  getVirtualServer() const;
    const VirtualServer::Location&  getLocationConfig() const;
    
    void  setServerConfig(const VirtualServer& srv_conf);

    //Call it only when _srv_config is set
    void  setLocationConfig(const std::string& uri);

    void  setResponse(const HTTPVersion& version, const uint16_t code,
        const std::string& reason, const headerMap_t& headers,
        const std::string& body);
    
    bool  operator==(const HTTPStream& rhs);
};

#endif
