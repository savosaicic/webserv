#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "VirtualServer.hpp"
#include <HTTPVersion.hpp>
#include <string>
#include <map>
#include <http_utils.hpp>

class HTTPResponse
{
  public:
    HTTPResponse();
    HTTPResponse(const HTTPResponse& src);
    HTTPResponse(const std::string& cgi_output);
    HTTPResponse(const VirtualServer::Redirection& redirection,
        const HTTPVersion& version = HTTPVersion(1, 1));
    HTTPResponse(const int status, const std::string& body,
        const std::string& content_type = mime_types::application_octet_stream,
        const HTTPVersion& version = HTTPVersion(1, 1));
    ~HTTPResponse();

    HTTPResponse& operator=(const HTTPResponse& rhs);

    void  setVersion(const HTTPVersion& version);
    void  setStatus(const int status);
    void  setReason(const std::string& reason);
    void  setHeaders(const std::map<std::string, std::string>& headers);
    void  setBody(const std::string& body);

    void  addHeader(const std::string& name, const std::string& value);

    const HTTPVersion&  getVersion() const;
    int                 getStatusCode() const;
    const std::string&  getReason() const;
    const std::map<std::string, std::string>& getHeaders() const;
    const std::string&  getBody() const;

    const std::string&  raw();
    int                 size() const;

  private:
    HTTPVersion   _version;
    int           _status;
    std::string   _reason;
    std::map<std::string, std::string> _headers;
    std::string _body;

    // Once all the Response is set
    // a call to raw() will set the _raw member
    // to the final string containing the response
    std::string _raw;
};

#endif
