#include "TCPStream.hpp"
#include "VirtualServer.hpp"
#include <HTTPServer.hpp>
#include <default_conf_value.hpp>

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

void  HTTPServer::_handleConnection(TCPStream& stream)
{
  if (stream.canRead())
  {
    if (stream.read() == 0)
    {
      std::cout << "disconnected client" << std::endl;
      _listener.dropClient(stream);
    }
    else
    {
      std::cout << "recived from: " << stream.getIPAddr().toStr()
				<< ": " << std::endl;
      std::cout << stream.getInMessage() << std::endl << std::endl;
      _listener.registerClientEvent(stream, EPOLLOUT);
    }
  }
  else if (stream.canWrite())
  {
    stream.write("HTTP/1.1 200 Ok\r\n\r\n");
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
