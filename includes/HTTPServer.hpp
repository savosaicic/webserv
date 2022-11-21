#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "IPAddress.hpp"
#include "TCPStream.hpp"
#include <TCPListener.hpp>
#include <Config.hpp>
#include <string>
#include <signal.h>
#include <HTTPStream.hpp>
#include <map>
#include <http_utils.hpp>

extern volatile sig_atomic_t listening;

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

    void  logRequest(const IPAddress& remote, const HTTPRequest& request,
        int status);

  private:
    void  _init();
    void  _handleConnection(TCPStream& stream);
    void  _handleRequest(HTTPStream& stream);
    void  _setStreamResponse(HTTPStream& stream,
        int status, const std::string& body,
        const std::string& content_type = mime_types::application_octet_stream);

  private:
    TCPListener _listener;
    std::map<int,HTTPStream> _active_client;
    Config      _config;
    
};

#endif
