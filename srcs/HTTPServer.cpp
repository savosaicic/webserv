#include "TCPStream.hpp"
#include "VirtualServer.hpp"
#include <HTTPServer.hpp>
#include <default_conf_value.hpp>
#include <exception>
#include <iostream>
#include <sys/epoll.h>
#include <utility>
#include <ctime>
#include <utils.hpp>

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

void  HTTPServer::_handleRequest(HTTPStream& stream)
{
  // This is temporary (waiting for HTTPRequest to be finished)
  stream._request = HTTPRequest(stream.tcp().getInMessage());
  std::cout << stream._request << std::endl;

/* TODO:
* Here, we should normaly check if the request is not complete.
* If so we should just return and not register to POLLOUT
* and stay registered to POLLIN 
* because we need to wait for more data to come
*/
  if (!stream.getRequest().isValid() /*isComplete*/)
  {
    std::cout << "invalid request\n";
    stream.setResponse(stream.getVirtualServer().getErrorPage(500));
    _listener.unRegisterClientEvent(stream.tcp(), EPOLLIN);
    _listener.registerClientEvent(stream.tcp(), EPOLLOUT);
    logRequest(stream.tcp().getIPAddr(), stream.getRequest(), 500);
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
    stream.setResponse(stream.getVirtualServer().getErrorPage(500));
    _listener.unRegisterClientEvent(stream.tcp(), EPOLLIN);
    _listener.registerClientEvent(stream.tcp(), EPOLLOUT);
    logRequest(stream.tcp().getIPAddr(), stream.getRequest(), 500);
    return ;
  }

  //If the VirtualServer have a redirection, no need to go further
  // Write something cleaner than this
/*
  if (stream.getVirtualServer().isThereRedirection())
  {
    stream.setResponse(
        buildRedirectionMessage(stream.config().server().redirection)
    );
    stream.unRegisterClientEvent(EPOLLIN);
    stream.registerClientEvent(EPOLLOUT);
    //Log request here ?
    return ;
  }
*/

  //Get the right Location config block based on URI
  //The VirtualServer server need to be already set
  stream.setLocationConfig(stream.getRequest().getUri());

  //Check if Method is allowed in this context
  if (!stream.getLocationConfig().isMethodAllowed(stream.getRequest().getMethod()))
  {
    stream.setResponse(
        stream.getVirtualServer().getErrorPage(405)
    );
    _listener.unRegisterClientEvent(stream.tcp(), EPOLLIN);
    _listener.registerClientEvent(stream.tcp(), EPOLLOUT);
    logRequest(stream.tcp().getIPAddr(), stream.getRequest(), 500);
    return ;
  }

  // Append URI to location path
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
