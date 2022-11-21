#include "TCPStream.hpp"
#include "VirtualServer.hpp"
#include <HTTPServer.hpp>
#include <default_conf_value.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/epoll.h>
#include <utility>
#include <ctime>
#include <utils.hpp>
#include <HTTPResponse.hpp>

#include <stdlib.h>

volatile sig_atomic_t listening = true;

HTTPServer::HTTPServer()
{
}

HTTPServer::HTTPServer(const HTTPServer& src) : _listener(src._listener),
  _config(src._config)
{
}

HTTPServer::~HTTPServer()
{
}
    
HTTPServer& HTTPServer::operator=(const HTTPServer& rhs)
{
  if (this != &rhs)
  {
    _listener = rhs._listener;
    _config = rhs._config;
  }
  return *this;
}

Config&  HTTPServer::config()
{
  return _config;
}

// This function is temporary and need to be somehow in the HTTPRequest class
std::string getRootedURI(std::string uri, const std::string& location,
    const std::string& docroot)
{
  return docroot + "/" + uri.erase(0, location.size());
}

void  HTTPServer::_setStreamResponse(HTTPStream& stream,
    const int status, const std::string& body,
    const std::string& content_type)
{
  HTTPResponse response(status, body, content_type);

  response.addHeader(http_headers::server, "webserv 0.1");
  response.addHeader(http_headers::connection, "close");

  stream.tcp().setOutMessage(response.raw());
  _listener.unRegisterClientEvent(stream.tcp(), EPOLLIN);
  _listener.registerClientEvent(stream.tcp(), EPOLLOUT);
  logRequest(stream.tcp().getIPAddr(), stream.getRequest(), status);
}

void  HTTPServer::_handleRequest(HTTPStream& stream)
{
  // This is temporary (waiting for HTTPRequest to be finished)
  stream._request = HTTPRequest(stream.tcp().getInMessage());

  // If request is not complete, wait for more data
  if (!stream.getRequest().isComplete())
    return ;

  if (!stream.getRequest().isValid())
  {
    std::cout << "invalid request\n";
    _setStreamResponse(stream, status_code::http_bad_request,
        stream.getVirtualServer().getErrorPage(status_code::http_bad_request),
        mime_types::text_html);
    return ;
  }

  //Get the right VirtualServer config block
  //(If header Host is missing or wrong, use default server)
  // HTTPRequest::setServerConfigBlock(Config c, HTTPRequest s)
  try {
    stream.setServerConfig(_config.getServer(
          stream.tcp().getHostIPAddr().toStr(),
          stream.tcp().getHostIPAddr().port(),
          "0.0.0.0"));
  } catch (const std::exception& e) {
    // Server block not found (this is temporary)
    std::cout << "server block not found\n";
    _setStreamResponse(stream, status_code::http_internal_error,
        stream.getVirtualServer().getErrorPage(status_code::http_internal_error),
        mime_types::text_html);
    return ;
  }

  //If the VirtualServer have a redirection, no need to go further
  // Write something cleaner than this
  if (stream.getVirtualServer().isRedirectionSet())
  {
    _setStreamResponse(stream, status_code::http_moved_permanently,
        std::string());
    return ;
  }

  //Get the right Location config block based on URI
  //The VirtualServer server need to be already set
  try
  {
    stream.setLocationConfig(stream.getRequest().getUri());
  }
  catch (const std::exception& e)
  {
    std::cout << "location block not found\n";
    _setStreamResponse(stream, status_code::http_internal_error,
        stream.getVirtualServer().getErrorPage(
          status_code::http_internal_error), mime_types::text_html);
    return ;
  }

  //If the Location have a redirection, no need to go further
  if (stream.getLocationConfig().isRedirectionSet())
  {
    _setStreamResponse(stream, status_code::http_moved_permanently,
        std::string());
    return ;
  }

  //Check if Method is allowed in this context
  if (!stream.getLocationConfig().isMethodAllowed(stream.getRequest().getMethod()))
  {
    _setStreamResponse(stream, status_code::http_not_allowed,
        stream.getVirtualServer().getErrorPage(status_code::http_not_allowed),
        mime_types::text_html);
    return ;
  }

  // Append URI to location path
  std::string rooted_uri = getRootedURI(stream.getRequest().getUri(),
      stream.getLocationConfig().getLocationPath(),
      stream.getLocationConfig().getDocroot());

  std::ifstream file(rooted_uri.c_str(), std::ios::binary | std::ios_base::in);
  //TODO: check permissions
  if (!file.is_open())
  {
    _setStreamResponse(stream, status_code::http_not_found,
        stream.getVirtualServer().getErrorPage(status_code::http_not_found),
        mime_types::text_html);
    return ;
  }
  std::string body =
    std::string(std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>());

  //TODO, determine the content_type based on rooted_uri extension
  _setStreamResponse(stream, status_code::http_ok, body,
      mime_types::text_html);
  return ;
}

void  HTTPServer::_handleConnection(TCPStream& stream)
{
  if (stream.canRead())
  {
    if (stream.read() == 0)
    {
      std::cout << "connection reset by peer: " <<
        stream.getIPAddr().toStr() << std::endl;
      _active_client.erase(stream.getFd());
      _listener.dropClient(stream);
    }
    else
    {
      // Here we need to check if the client is active
      // (meaning that the client already send a request that wasn't complete)
      // if he exist, append the new data somehow

      // If client already exist, the map wont create a new one
      // and will return the existing one
      _handleRequest(
        _active_client.insert(std::make_pair(stream.getFd(),
            HTTPStream(stream))).first->second);
    }
  }
  else if (stream.canWrite())
  {
    stream.write(stream.getOutMessage());
    _active_client.erase(stream.getFd());
    _listener.dropClient(stream);
  }
}

void  HTTPServer::_init()
{
  if (_config.isDefined() == false)
    _config.parseConfigFile(DEFAULT_CONFIG_FILE);

  for (std::vector<VirtualServer>::const_iterator virtual_server =
      _config.getServerList().begin();
      virtual_server != _config.getServerList().end(); ++virtual_server)
  {
    _listener.bind(virtual_server->getListeningHost().c_str(),
        virtual_server->getListeningPort().c_str());
  }
  _listener.listen();
}

void HTTPServer::start()
{
  std::vector<TCPStreamManager::iterator> incoming_streams;

  _init();
  while (listening)
  {
    incoming_streams = _listener.incoming();
    for (std::vector<TCPStreamManager::iterator>::iterator stream =
             incoming_streams.begin();
         stream != incoming_streams.end(); ++stream)
    {
      _handleConnection((*stream)->second);
    }
  }
}

void  HTTPServer::logRequest(const IPAddress& remote,
    const HTTPRequest& request, int status)
{
  std::cerr << remote.toStr() << " - " <<
    "[" << getTimeToStr() << "] \"" <<
    request.getMethod() << " " <<
    request.getUri() << " " << "HTTP/" <<
    request.getHTTPVersion().first << "." <<
    request.getHTTPVersion().second << "\" " << status << std::endl;
}
