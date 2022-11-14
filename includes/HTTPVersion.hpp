#ifndef HTTP_VERSION_HPP
#define HTTP_VERSION_HPP

#include <string>
#include <utility>
#include <stdint.h>

class HTTPVersion
{
  public:
    HTTPVersion();
    HTTPVersion(const HTTPVersion& src);
    HTTPVersion(const std::pair<uint16_t, uint16_t>& version);
    HTTPVersion(const uint16_t major, const uint16_t minor);
    ~HTTPVersion();

    HTTPVersion&  operator=(const HTTPVersion& rhs);

    bool  isSet() const;
    const std::string&  getPrefix() const;
    uint16_t  getMajor() const;
    uint16_t  getMinor() const;

    void  setMajor(uint16_t major);
    void  setMinor(uint16_t minor);

    bool  operator==(const HTTPVersion& rhs);
    bool  operator!=(const HTTPVersion& rhs);
    bool  operator<(const HTTPVersion& rhs);
    bool  operator>(const HTTPVersion& rhs);
    bool  operator<=(const HTTPVersion& rhs);
    bool  operator>=(const HTTPVersion& rhs);

  private:
    static const std::string&  _prefix;
    std::pair<uint16_t, uint16_t> _version;

};

#endif
