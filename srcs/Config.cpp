#include "Config.hpp"
#include "VirtualServer.hpp"
#include "HTTPRequest.hpp"
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <ostream>
#include <string>
#include <bitset>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <sstream>

#define X(index, name, allowed_context, parse_fun) index,
    enum Directive { DIRECTIVES };
#undef X

#define X(index, name, allowed_context, parse_fun) name,
    static char const *directive_name[] = { DIRECTIVES };
#undef X

#define X(index, name, allowed_context, parse_fun) allowed_context,
    static int context_flag[] = { DIRECTIVES };
#undef X

int context;

Config::Config() : _defined(false)
{}

Config::Config(const Config& src) : _defined(src._defined),
  _server_list(src._server_list)
{}

Config::~Config()
{}

Config& Config::operator=(const Config& rhs)
{
  if (this != &rhs)
  {
    _defined = rhs._defined;
    _server_list = rhs._server_list;
  }
  return *this;
}

bool check_ip( std::string ip )
{
  int dot;
  std::vector<int> octet_list;
  std::string octet;
  int i = 0;

  dot = std::count(ip.begin(), ip.end(), '.');
  if (dot != 3)
    return (false);

  while (i++ < 4)
  {
    octet = ip.substr(0, ip.find_first_of('.'));
    ip.erase(0, ip.find_first_of('.') + 1);
    if ((atoi(octet.c_str()) < 0) || (atoi(octet.c_str()) > 255))
      throw std::runtime_error("Non valid octet in ip adress");
  }
  return (true);
}

bool check_port( std::string port )
{
  if ((size_t)std::count_if(port.begin(), port.end(), isdigit) != port.size())
    return (false);
  int n = atoi(port.c_str());
  if (n < 1 || n > 65535)
    return (false);
  return true;
}

bool  check_extention( const std::string &current_word )
{
  if (current_word[0] != '.')
    return (false);
  long unsigned int n = std::count_if(current_word.begin() + 1, current_word.end(), isalpha);
  if (n + 1 != current_word.size())
    return (false);
  return (true);
}

bool isuri(const std::string &current_word)
{
  long unsigned int slash;
  long unsigned int alnum;
  long unsigned int underscore;
  long unsigned int hyphen;
  long unsigned int dot;

  slash = std::count(current_word.begin(), current_word.end(), ('/'));
  alnum = std::count_if(current_word.begin(), current_word.end(),isalnum);
  hyphen = std::count(current_word.begin(), current_word.end(),'-');
  dot = std::count(current_word.begin(), current_word.end(),'.');
  underscore = std::count(current_word.begin(), current_word.end(),'_');
std::cout << underscore << " == " << current_word.size() << std::endl;
  if (slash + alnum + underscore + hyphen + dot != current_word.size())
    return (false);
  return (true);
}

bool ispath(const std::string &current_word)
{
  if (*current_word.begin() != '/' || (current_word.size() != 1 && *current_word.rbegin() == '/'))
    return (false);
  return (isuri(current_word));
}

void parse_listen( const std::string &previous_word, const std::string &current_word, int words_count, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection )
{
  (void)previous_word;
  (void)server;
  (void)location;
  (void)redirection;
  int dot;
  int dotdot;
  std::string ip;
  std::string host;
  std::string port;
std::cout << "current_word is " << current_word << std::endl;
  if (words_count == 1)
    return ;
  else if (words_count > 2)
    throw std::runtime_error("Wrong number of words in listen directive line\n");
  dotdot = std::count(current_word.begin(), current_word.end(), ':');
  dot = std::count(current_word.begin(), current_word.end(), '.');
  if (dotdot == 1)
  {
    if (dot != 3)
      throw std::runtime_error("Non valid ip format in listen directive\n");
    
    ip = current_word.substr(0, current_word.find_first_of(':')); //std::strtok((char*)current_word.c_str(), ":");
    port = current_word.substr(current_word.find_first_of(':') +  1); //std::strtok(NULL, ":");
    if (check_ip(ip) == false)
      throw std::runtime_error("Wrong IP adress format in listen directive\n");
    if (check_port(port) == false)
      throw std::runtime_error("Wrong port format in listen directive\n");
  }
  else if (!dotdot)
  {
    if (check_port(current_word) == false)
      throw std::runtime_error("Wrong port format in listen directive\n");
    port = current_word.c_str();
    ip.clear();
  }
  else
    throw std::runtime_error("Wrong format in listen directive line\n");
  if (!ip.empty())
    server.setListeningHost(ip);
  server.setListeningPort(port);
  return ;
}

void parse_server( const std::string &previous_word, const std::string &current_word, int words_count, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection )
{
  (void)previous_word;
  (void)server;
  (void)location;
  (void)redirection;
  if (words_count == 1)
    return ;
  else if (words_count > 2)
    throw std::runtime_error("Too much words in server directive line\n");
  else if (current_word != "{")
      throw std::runtime_error("Wrong word in server directive line. expected:'{'\n");
  return ;
}

void parse_cgi( const std::string &previous_word, const std::string &current_word, int words_count, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection )
{
  (void)previous_word;
  (void)server;
  (void)location;
  (void)redirection;

  std::string extension;

  if (words_count == 1)
    return ;
  if (words_count > 3)
    throw std::runtime_error("Too much words in cgi directive line\n");
  if (words_count == 2)
  {
    if (!check_extention(current_word))
      throw std::runtime_error("Wrong extention format in cgi directive line\n");
    return ;
  }
  if (words_count == 3 && !ispath(current_word))
    throw std::runtime_error("Wrong path format in cgi directive line\n");
  server.addCgiExtension(previous_word, current_word);
  
}

void parse_location( const std::string &previous_word, const std::string &current_word, int words_count, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection )
{
  (void)previous_word;
  (void)server;
  (void)redirection;

  if (words_count == 1)
    return ;
  else if (words_count == 2 && !ispath(current_word))
    throw std::runtime_error("Error while parsing location directive line\n");
  else if (words_count == 3 && current_word != "{")
    throw std::runtime_error("Error while parsing locarion directive line\n");
  else if (words_count > 3)
    throw std::runtime_error("Error while parsing locafion directive line\n");
  if (words_count == 2) {
    location.setLocationPath(current_word);
  }
}

void parse_server_name( const std::string &previous_word, const std::string &current_word, int words_count, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection )
{
  (void)previous_word;
  (void)location;
  (void)redirection;
  (void)words_count;

  if (words_count == 1)
    return ;

  // uint alnum = std::count_if(current_word.begin(), current_word.end(), isalnum);
  // uint dot = std::count_if(current_word.begin(), current_word.end(), ispunct);
  
  // if (alnum + dot == current_word.size() && words_count > 1)
    server.addServerName(current_word);
std::cout << "SERVER NAME ADDED!!! ---- " << current_word << std::endl;
}

void parse_forbid_method( const std::string &previous_word, const std::string &current_word, int words_count, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection )
{
  (void)previous_word;
  (void)server;
  (void)redirection;
  (void)words_count;  

  if (current_word == "GET")
    location.forbidMethod(HTTPRequest::GET);
  else if (current_word == "POST")
    location.forbidMethod(HTTPRequest::POST);
  else if (current_word == "DELETE")
    location.forbidMethod(HTTPRequest::DELETE);
}

void parse_root( const std::string &previous_word, const std::string &current_word, int words_count, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection )
{
  (void)previous_word;
  (void)server;
  (void)redirection;
std::cout << current_word << std::endl;
  if (words_count > 2)
    throw std::runtime_error("Too much words in root directive line");
  if (words_count == 2 ) {
    if (!ispath(current_word))
      throw std::runtime_error("Wrong path format in root directive line");
    if (std::count(current_word.begin(), current_word.end(), '.'))
      throw std::runtime_error("No dots accepted in root path");
    location.setDocroot(current_word);
  }
}

void parse_return( const std::string &previous_word, const std::string &current_word, int words_count, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection )
{
  (void)previous_word;
  (void)server;
  (void)location;

  if (words_count > 3)
    throw std::runtime_error("Too much words in return directive line");
  if (words_count == 3 ) {
    uint dotdot = count(current_word.begin(), current_word.end(), ':');
    if (dotdot) {
      // current_word.substr(current_word.find_first_of(':') + 1); // std::strtok((char*)current_word.c_str(), ":");
      if (!isuri(current_word.substr(current_word.find_first_of(':') + 1)) || dotdot > 1)
        throw std::runtime_error("Wrong uri format in return directive line");
    }
    redirection.setLocation(current_word);
    if (context == LOCATION)
      location.setRedirection(redirection);
    else
      server.setRedirection(redirection);
  }
  if (words_count == 2) {
    uint digits = std::count_if(current_word.begin(), current_word.end(), isdigit);
    int status_code = atoi(current_word.c_str());

    if (digits != current_word.size())
      throw std::runtime_error("Status code must be a number in return directive line");
    if (status_code < 300 || status_code > 399)
      throw std::runtime_error("Status code must be a number between 300 and 399 for redirection. Error in return directive line");
    redirection.setStatusCode(status_code);
  }
}
void parse_client_max_body_size( const std::string &previous_word, const std::string &current_word, int words_count, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection )
{
  (void)previous_word;
  (void)location;
  (void)redirection;
  (void)words_count;  

  if (words_count < 2 || words_count > 2)
    return ;

  uint max_body_size = 0;
  std::string copy = current_word;

  int mult;
  if (std::isalpha(*current_word.rbegin())) {
    if (*current_word.rbegin() == 'B' || *current_word.rbegin() == 'b')
      mult = 1;
    else if (*current_word.rbegin() == 'M' || *current_word.rbegin() == 'm')
      mult = 1000;
    else if (*current_word.rbegin() == 'G' || *current_word.rbegin() == 'g')
      mult = 1000000;
    else
      throw std::runtime_error("The mesure unit of the client max body size must be \'B\' for bytes, \'M\' for mega bytes or \'G\' for giga bytes");
    for (std::string::reverse_iterator it = copy.rbegin() + 1; it != copy.rend(); ++it) {
      if (!isdigit(*it))
        throw std::runtime_error("Size of max body must be a number");
      else {
        max_body_size += (*it - 48) * mult;
        mult *= 10;
      }
    }
  }
  if (max_body_size)
    server.setClientMaxBodySize(max_body_size);
}
void parse_error_page( const std::string &previous_word, const std::string &current_word, int words_count, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection )
{
  (void)location;
  (void)redirection;

  uint16_t error_code;

  if (words_count == 1)
    return ;
  if (words_count == 2) {
    uint digits = std::count_if(current_word.begin(), current_word.end(), isdigit);
    if (digits != 3 || digits != current_word.size())
      throw std::runtime_error("Code for error page must be a three digits number");
    error_code = atoi(current_word.c_str());
    if (error_code < 400 || error_code > 599)
      throw std::runtime_error("Code for error page must be between 400 and 599");
  }
  else if (words_count == 3) {
    if (!ispath(current_word))
      throw std::runtime_error("Wrong error page path format");
    error_code = atoi(previous_word.c_str());
    server.addErrorPage(error_code, current_word);
  }
  else
    throw std::runtime_error("Too much words in error page directive line");
}
void parse_autoindex( const std::string &previous_word, const std::string &current_word, int words_count, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection )
{
  (void)previous_word;
  (void)server;
  (void)redirection;

  if (words_count == 1)
    return ;
  else if (words_count == 2) {
    if (current_word == "on")
      location.setAutoindex(true);
    else if (current_word == "off")
      location.setAutoindex(false);
    else
      throw std::runtime_error("Autoindex settings must be either \'on\' or \'off\'");
  }
  else
    throw std::runtime_error("Too much words in autoindex directive line");
}
void parse_index( const std::string &previous_word, const std::string &current_word, int words_count, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection )
{
  (void)previous_word;
  (void)server;
  (void)location;
  (void)redirection;
  (void)current_word;
  (void)words_count;  

  if (words_count == 1)
    return ;
  else if (words_count == 2) {
    if (!isuri(current_word))
      throw std::runtime_error("Wrong path format in index directive line");
    location.addIndexFileToList(current_word);
  }
  else
    throw std::runtime_error("Too much words in index directive line");
}
void parse_unknown( const std::string &previous_word, const std::string &current_word, int words_count, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection )
{
  (void)previous_word;
  (void)server;
  (void)location;
  (void)redirection;
  (void)current_word;
  (void)words_count;  
  return ;
}

#define X(index, name, allowed_context, parse_fun) parse_fun,
    static void (*parsing_functions[])(const std::string &,  const std::string &, int, VirtualServer&, VirtualServer::Location&, VirtualServer::Redirection&) = { DIRECTIVES };
#undef X 

int Config::checkDirective( std::string first_word )
{
  int index = 0;
  while (index < LAST)
  {
    if (directive_name[index] == first_word)
      return (index);
    ++index;
  }
  return (index);
}

int Config::checkContext( int index)
{
  if((context_flag[index] & context) == context)
    return (1);
  return (0);
}

void Config::parse_line (char* str, VirtualServer &server, VirtualServer::Location &location, VirtualServer::Redirection &redirection)
{
  std::string   first_word;
  char*         previous_word;
  char*         current_word;
  int           index;
  
  first_word    = std::strtok(str, "\t \n;");
  current_word  = (char*)first_word.c_str();
  previous_word = current_word;
  index         = checkDirective(first_word);

  if (index != LAST && !checkContext(index))
    throw std::runtime_error("ConfigFile context error\n");
  for (int words_count = 1; current_word != NULL; ++words_count)
  {
    if (*current_word == '#')
      break;
    else if (current_word[0] == '{')
      context = context << 1;
    else if (current_word[0] == '}')
    {
      context = context >> 1;
      if (context == HTTP) {
        _server_list.push_back(server);
        server = VirtualServer();
      } 
      if (context == SERVER) {
        server.addLocation(location);
        location = VirtualServer::Location();
      }
    } 
    else if (index == LAST)
      throw std::runtime_error("Unknown ConfigFile Directive\n");
    if (!context || context > LOCATION)
      throw std::runtime_error("ConfigFile out of context error\n");
std::cout << current_word << " in " << std::bitset<4>(context) << std::endl;
    (*parsing_functions[index])(previous_word, current_word, words_count, server, location, redirection);
    previous_word = current_word;
    current_word = strtok( NULL, "\t \n;");
  }
}

void  Config::parseConfigFile(const char* filename)
{
  std::ifstream file(filename);
  std::string   line;
  char*         str;
  VirtualServer server;
  VirtualServer::Location location;
  VirtualServer::Redirection redirection;

  context = HTTP;
  if (!file.is_open())
  {
    std::cerr << "Can't open file: " << filename
      << std::endl << std::strerror(errno) << std::endl;
    throw std::ifstream::failure("");
  }

  while (std::getline(file, line))
  {
    if ((std::count_if(line.begin(), line.end(), ::isspace) == (int)line.length()))
      continue;
    // if (line.length())
    // {
    //   str = (char*)malloc(line.length() + 1);
    //   str[line.length()] = 0;
    //   std::strcpy(str, line.c_str());
    // }
    str = strdup(line.c_str());
    parse_line(str, server, location, redirection);
    if (line.length())
      free(str);
  }
  if (context != HTTP)
    throw std::runtime_error("error: ConfigFile do not end in HTTP context");
  _defined = true;
  (void)context_flag;
}

const VirtualServer&  Config::getServer(const std::string& listening_addr,
    const std::string& port, const std::string& server_name) const
{
  for (std::vector<VirtualServer>::const_iterator it = _server_list.begin();
      it != _server_list.end(); ++it)
  {
    if (it->getListeningHost() == listening_addr &&
        it->getListeningPort() == port && it->isServerNameSet(server_name))
      return *it;
  }
  throw std::logic_error("getServer() failed");
}

const std::vector<VirtualServer>& Config::getServerList()
{
  return _server_list;
}

bool  Config::isDefined()
{
  return _defined;
}
