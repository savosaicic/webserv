/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiju-du <abiju-du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 16:31:11 by sasaicic          #+#    #+#             */
/*   Updated: 2022/11/22 15:33:43 by abiju-du         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "VirtualServer.hpp"
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

#define X(index, name, allowed_context) index,
    enum Directive { DIRECTIVES };
#undef X

#define X(index, name, allowed_context) name,
    static char const *directive_name[] = { DIRECTIVES };
#undef X

#define X(index, name, allowed_context) allowed_context,
    static int context_flag[] = { DIRECTIVES };
#undef X    

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

// int Config::checkContext( std::string line )
// {
  
// }

void  Config::parseConfigFile(const char* filename)
{
  std::ifstream file(filename);
  std::string   line;
  std::string   first_word;
  char*   current_word;
  int           context = HTTP;
  int           index;
  std::vector <std::string> tokens;
  char*          str;

  if (!file.is_open())
  {
    std::cerr << "Can't open file: " << filename
      << std::endl << std::strerror(errno) << std::endl;
    throw std::ifstream::failure("");
  }

  while (std::getline(file, line))
  {
    // checkContext(line);
    // line.erase(std::remove_if(line.begin(), line.end(), isspace), line.begin());
    // std::stringstream check1(line);
    // while (std::getline(check1, current_word, ' '))
    //   std::cout << current_word << "|";
    // std::cout << std::endl;
    if ((std::count_if(line.begin(), line.end(), ::isspace) == (int)line.length()))
      continue;
    if (line.length())
    {
      str = (char*)malloc(line.length() + 1);
      str[line.length() + 1] = 0;
      std::strcpy(str, line.c_str()); 
    }
    first_word = std::strtok(str, "\t \n;");
    std::cout << line << std::endl;
    std::cout << "<<" << first_word << ">>" << std::endl;
    current_word = (char*)first_word.c_str();
    index = checkDirective(first_word);
std::cout << "index is: " << directive_name[index] << std::endl;
    if (index != LAST && !checkContext(index, context))
      throw std::runtime_error("ConfigFile context error\n");
    while (current_word != NULL)
    {
      if (current_word[0] == '#')
        break;
      else if (current_word[0] == '{')
        context = context << 1;
      else if (current_word[0] == '}')
        context = context >> 1;
      else if (index == LAST)
        throw std::runtime_error("Unknown ConfigFile Directive\n");
      if (!context || context > LOCATION)
        throw std::runtime_error("ConfigFile out of context error\n");
      std::cout << "<" << current_word << "> in context: " << std::bitset<4>(context) << std::endl;
      current_word = strtok( NULL, "\t \n;");
    }
      // memset(&str[0], 0, line.length());
    if (line.length())
      free(str);
  }
    std::cout << line << std::endl;
  if (context != HTTP)
    throw std::runtime_error("error: ConfigFile do not end in HTTP context");
  _defined = true;
  (void)context_flag;
std::cout << directive_name[AUTOINDEX] << " - " << std::bitset<4>(context_flag[AUTOINDEX]) << std::endl;
std::cout << directive_name[CLIENT_MAX_BODY_SIZE] << " - " << std::bitset<4>(context_flag[CLIENT_MAX_BODY_SIZE]) << std::endl;
}

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
int Config::checkContext( int index, int context )
{
  if((context_flag[index] & context) == context)
    return (1);
  return (0);
}

const VirtualServer&  Config::getServer(const std::string& listening_addr,
    const std::string& port, const std::string& server_name) const
{
  //TODO: if server_name is empty (If no Host: name was sent)
  // return the default server for that port
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