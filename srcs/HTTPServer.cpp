#include "HTTPRequest.hpp"
#include "HTTPStream.hpp"
#include "TCPStream.hpp"
#include "VirtualServer.hpp"
#include "http_utils.hpp"
#include <HTTPServer.hpp>
#include <default_conf_value.hpp>
#include <exception>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <sys/epoll.h>
#include <utility>
#include <ctime>
#include <utils.hpp>
#include <HTTPResponse.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <ws_syscall_error.hpp>
#include <ext/stdio_filebuf.h>
#include <fcntl.h>
#include <dirent.h>
#include <CGIRequest.hpp>

#include <stdlib.h>

volatile sig_atomic_t listening = true;

HTTPServer::HTTPServer()
{
}

HTTPServer::HTTPServer(char **envp) : _envp(envp)
{}

HTTPServer::HTTPServer(const HTTPServer& src) : _listener(src._listener),
  _config(src._config), _envp(src._envp)
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
    _envp = rhs._envp;
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

std::string  _buildAutoIndexPage(const VirtualServer& server_conf,
    const VirtualServer::Location& location_conf,
    const std::string& rooted_uri)
{
  DIR* directory = opendir(rooted_uri.c_str());
  if (!directory)
    return server_conf.getErrorPage(404); //TODO: or 403

  struct dirent *entry;

  //erase first '/'
  std::string location(location_conf.getLocationPath().substr(1));

  std::stringstream html;

  html << "<html><body style=\"background-color: #EDE6D9;\"><pre>";
  while ((entry = readdir(directory))) {
    std::string slash = entry->d_type & DT_DIR ? "/" : "";
    html << "<a href=\"" <<
      (location + entry->d_name + slash) <<
      "\">" << (location + entry->d_name + slash) <<
      "</a>" << std::endl;
  }
  html << "</body></pre></html>";
  closedir(directory);
  return html.str();
}

int HTTPServer::_openURI(const VirtualServer& server_conf,
    const VirtualServer::Location& location_conf,
    std::string& rooted_uri, std::ifstream* fstream)
{
  // Search indexes for this location (index directive)
  if (*rooted_uri.rbegin() == '/') {
    struct stat statbuf;
    VirtualServer::Location::index_list_t::const_iterator it;
    for (it = location_conf.indexBegin();
        it != location_conf.indexEnd(); ++it) {
      if (stat((rooted_uri + *it).c_str(), &statbuf) == -1)
        continue;
      if (statbuf.st_mode & S_IFREG && !(statbuf.st_mode & S_IRUSR))
        return 1;
      else if (statbuf.st_mode & S_IFREG && statbuf.st_mode & S_IRUSR) {
        rooted_uri.append(*it);
        fstream->open(rooted_uri.c_str(),
            std::ios::in | std::ios::binary);
        return 0;
      }
    }
    if (location_conf.getAutoindex()) {
      if (stat((rooted_uri).c_str(), &statbuf) != -1) {
        if (statbuf.st_mode & S_IFDIR && statbuf.st_mode & S_IXUSR) {
          return 2;
        }
        return 0;
      }
    }
  }
  else { // open normal file
    struct stat statbuf;
    if (stat(rooted_uri.c_str(), &statbuf) == -1)
      return 0;
    if (location_conf.getAutoindex() &&
        statbuf.st_mode & S_IFDIR && statbuf.st_mode & S_IXUSR)
      return 2;
    if (server_conf.isCgiExtension(
          rooted_uri.substr(rooted_uri.find_last_of('.'))))
      return 3;
    if (statbuf.st_mode & S_IFREG && !(statbuf.st_mode & S_IRUSR))
      return -1;
    fstream->open(rooted_uri.c_str(), std::ios::in | std::ios::binary);
  }
  return 0;
}

void  HTTPServer::_setStreamRedirectionResponse(HTTPStream& stream,
    const VirtualServer::Redirection& redirection)
{
  HTTPResponse response(redirection);
  response.addHeader(http_headers::server, "webserv 0.1");
  response.addHeader(http_headers::connection, "close");
  stream.tcp().setOutMessage(response.raw());
  _listener.unRegisterClientEvent(stream.tcp(), EPOLLIN);
  _listener.registerClientEvent(stream.tcp(), EPOLLOUT);
  logRequest(stream.tcp().getIPAddr(), stream.getRequest(),
      redirection.statusCode());
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

  stream.setServerConfig(_config.getServer(
        stream.tcp().getHostIPAddr().toStr(),
        stream.tcp().getHostIPAddr().port(),
        "0.0.0.0")); // TODO: replace with stream.getHeaderContent("Host")

  //If the VirtualServer have a redirection, no need to go further
  // Write something cleaner than this
  if (stream.getVirtualServer().isRedirectionSet())
  {
    _setStreamRedirectionResponse(stream,
        stream.getVirtualServer().getRedirection());
    return ;
  }

  //Get the right Location config block based on URI
  //The VirtualServer server need to be already set
  stream.setLocationConfig(stream.getRequest().getUri());

  //If the Location have a redirection, no need to go further
  if (stream.getLocationConfig().isRedirectionSet())
  {
    _setStreamRedirectionResponse(stream,
        stream.getLocationConfig().getRedirection());
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

  std::ifstream ressource;
  int ret = _openURI(stream.getVirtualServer(),
      stream.getLocationConfig(), rooted_uri, &ressource);
  if (ret == 1) { // Permission denied
    _setStreamResponse(stream, status_code::http_forbidden,
        stream.getVirtualServer().getErrorPage(status_code::http_forbidden),
        mime_types::text_html);
    return ;
  }
  else if (ret == 2) { // Autoindex
    _setStreamResponse(stream, status_code::http_ok,
        _buildAutoIndexPage(stream.getVirtualServer(),
          stream.getLocationConfig(), rooted_uri),
        mime_types::text_html);
    return ;
  }
  else if (ret == 3) { // CGI
    CGIRequest cgi_request(stream.getRequest(), rooted_uri,
        stream.getVirtualServer().getCgiProg(
          rooted_uri.substr(rooted_uri.find_last_of('.'))));
    if (cgi_request.executeCGI(_envp)) {
      _setStreamResponse(stream, status_code::http_ok,
          cgi_request.getOutput());
    }
    else {
      _setStreamResponse(stream, status_code::http_internal_error,
          stream.getVirtualServer().getErrorPage(
            status_code::http_internal_error), mime_types::text_html);
    }
    return ;
  }
  else if (!ressource.is_open()) // Ressource not found
  {
    _setStreamResponse(stream, status_code::http_not_found,
        stream.getVirtualServer().getErrorPage(status_code::http_not_found),
        mime_types::text_html);
    return ;
  }
  std::string body =
    std::string(std::istreambuf_iterator<char>(ressource),
        std::istreambuf_iterator<char>());

  _setStreamResponse(stream, status_code::http_ok, body,
      mime_types::content_type(rooted_uri));
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
