#ifndef IP_ADRESS_HPP
#define IP_ADRESS_HPP

#include <string>
#include <sys/socket.h>

class IPAddress
{
  private:
    std::string      _ip_str;
    std::string      _port_str;
    struct sockaddr  _addr;
    socklen_t        _addrlen;
    bool             _defined;

  public:
    IPAddress();
    IPAddress(const IPAddress& src);
    IPAddress(const struct sockaddr addr, const socklen_t addrlen);
    ~IPAddress();

    IPAddress&  operator=(const IPAddress& rhs);

    const std::string& port() const;
    const std::string& toStr() const;
    const sockaddr    addr() const;
    socklen_t   addrlen() const;

    bool  isDefined() const;

    bool  operator==(const IPAddress& rhs);
};

#endif
