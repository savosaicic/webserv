#ifndef VIRTUAL_SERVER_HPP
#define VIRTUAL_SERVER_HPP

#include <stdint.h>
#include <vector>
#include <string>
#include <map>
#include "HTTPRequest.hpp"

class VirtualServer
{
  public:
    class Location;

  private:
    //Not sure about those types
    std::string _host;
    std::string _port;

    // <error_code, error_page>
    static std::pair<unsigned short, std::string> map_data[5];
    static const std::map<unsigned short, std::string>  _default_error_page_list;
    
    // <error_code, error_page_filename>
    std::map<uint16_t, std::string>  _error_page_file_list;

    unsigned int         _client_max_body_size;

    // <cgi_extension, cgi_bin_path>
    std::map<std::string, std::string> _cgi_list;

    std::vector<std::string>  _server_name_list;
  
    std::vector<Location>  _location_list;

   public:
    VirtualServer();
    VirtualServer(const VirtualServer& src);
    ~VirtualServer();

    VirtualServer&  operator=(const VirtualServer& rhs);

    const std::string&  getListeningHost() const;
    const std::string&  getListeningPort() const;
    const std::string   getErrorPage(const uint16_t error_num) const;
    uint64_t            getClientMaxBodySize() const;
    const Location&     getLocation(const std::string& uri_src) const;

    bool  isServerNameSet(const std::string& name) const;

    void  setListeningHost(const std::string& host);
    void  setListeningPort(const std::string& port);
    void  addLocation(const Location& location);
    void  addServerName(const std::string& name);
    void  addErrorPage(const uint16_t error_num, const std::string& filename);
    void  setClientMaxBodySize(const uint64_t size);

   public:
    class Location
    {
      private:
        std::string   _location;
        std::string   _docroot;
        bool          _autoindex;

        std::vector<std::string>  _index_list;

        std::vector<HTTPRequest::method_t>     _allowed_method_list;

      public:
        Location();
        Location(const Location& src);
        ~Location();

        Location& operator=(const Location& rhs);

        void  setLocationPath(const std::string &location);
        void  setDocroot(const std::string& docroot);
        void  setAutoindex(const bool autoindex);
        void  addIndexFileToList(const std::string& index);
        void  allowMethod(const HTTPRequest::method_t method);

        const std::string&  getLocationPath() const;
        const std::string&  getDocroot() const;
        bool	getAutoindex() const;
        bool	isIndexInList(const std::string& index) const;
        bool	isMethodAllowed(const HTTPRequest::method_t method) const;
    };


    class Redirection
    {
      private:
        std::string _location;
        int         _status_code;
        bool        _on;
  
      public:
        Redirection();
        Redirection(const Redirection& src);
        Redirection(const std::string& location, const int code);
        ~Redirection();

        Redirection&  operator=(const Redirection& rhs);

        bool  isActive() const;
        uint16_t  statusCode() const;
        const std::string& getLocation() const;

        void  setLocation(const std::string& location);
        void  setStatusCode(const uint16_t code);
        void  setRedirectionOn();
    };

   private:
    static const std::string _createErrorPage(const uint16_t code,
        const std::string& msg);
  
};

#endif
