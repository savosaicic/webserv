#ifndef CGI_RESPONSE_HPP
#define CGI_RESPONSE_HPP

#include "VirtualServer.hpp"
#include <HTTPVersion.hpp>
#include <string>
#include <map>
#include <http_utils.hpp>

class CGIResponse
{
  public:
    CGIResponse();
    CGIResponse(const CGIResponse& src);
    CGIResponse(const std::string& cgi_output,
        const HTTPVersion& version = HTTPVersion(1, 1));
    ~CGIResponse();

    CGIResponse& operator=(const CGIResponse& rhs);

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
    std::string  _cgi_output;

    // Once all the Response is set
    // a call to raw() will set the _raw member
    // to the final string containing the response
    std::string _raw;
};

#endif
