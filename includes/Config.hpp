#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <string>
#include "VirtualServer.hpp"

enum ContextFlag {
  OUT_OF_CONTEXT  = 0,      // 0000
  HTTP            = 1 << 0, // 0001
  SERVER          = 1 << 1, // 0010
  LOCATION        = 1 << 2  // 0100
};

#define DIRECTIVES \
X(SERV, "server", int(HTTP)) \
X(CGI, "cgi", int(SERVER)) \
X(LISTEN, "listen", int(SERVER)) \
X(LOC, "location", int(SERVER)) \
X(SERVER_NAME, "server_name", int(SERVER)) \
X(LIMIT_EXCEPT, "limit_except", int(LOCATION)) \
X(ROOT, "root", int(HTTP|SERVER|LOCATION)) \
X(RETURN, "return", int(SERVER|LOCATION)) \
X(CLIENT_MAX_BODY_SIZE, "client_max_body_size", int(HTTP|SERVER|LOCATION)) \
X(ERROR_PAGE, "error_page", int(HTTP|SERVER|LOCATION))\
X(AUTOINDEX, "autoindex",int(HTTP|SERVER|LOCATION)) \
X(INDEX, "index", int(HTTP|SERVER|LOCATION)) \
X(LAST, "unknown", 0)

class Config
{
  private:
    bool  _defined;
    std::vector<VirtualServer>  _server_list;

  public:
    Config();
    Config(const Config& src);
    ~Config();

    Config& operator=(const Config& rhs);

    void  parseConfigFile(const char* filename);
    int   checkContext(int index, int context);
    int   checkDirective( std::string first_word );
    #define X(index, name, allowed_context) \
      int parse_directive( std::string, #name ); \
      DIRECTIVES
    #undef X


    // This method will return a server with a given :
    // - Listening address (ip+port)
    // - a server_name (From http request header `Host`)
    const VirtualServer&  getServer(const std::string& listening_addr,
        const std::string& port, const std::string& server_name) const;

    const std::vector<VirtualServer>& getServerList();

    bool  isDefined();
};



#endif