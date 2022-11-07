#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "TCPStream.hpp"
#include <TCPListener.hpp>
#include <Config.hpp>
#include <string>

class HTTPServer
{
  public:
    HTTPServer();
    HTTPServer(const HTTPServer& src);
    ~HTTPServer();
    
    HTTPServer& operator=(const HTTPServer& rhs);

    Config&  config();

    // Main loop handling
    void  start();

  private:
    void  _init();
    void  _handleConnection(TCPStream& stream);

  private:
    TCPListener _listener;
    Config      _config;
    
};

#endif
