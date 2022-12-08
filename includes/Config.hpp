#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <string>
#include "VirtualServer.hpp"

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

    // This method will return a server with a given :
    // - Listening address (ip+port)
    // - a server_name (From http request header `Host`)
    const VirtualServer&  getServer(const std::string& listening_addr,
        const std::string& port, const std::string& server_name) const;

    const std::vector<VirtualServer>& getServerList();

    bool  isDefined();
};

#endif
