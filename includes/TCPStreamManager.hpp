#ifndef TCP_STREAM_MANAGER_HPP
#define TCP_STREAM_MANAGER_HPP

#include <vector>
#include "TCPStream.hpp"

class TCPListener;

class TCPStreamManager
{
  public:
    typedef std::vector<TCPStream>    container_type;
    typedef container_type::iterator  iterator;

  private:
    int&  _epfd;
    TCPListener&  _listener;

    container_type  _connected_clients;

  public:
    TCPStreamManager(int& epfd, TCPListener& listener);
    TCPStreamManager(const TCPStreamManager& src);
    ~TCPStreamManager();

    TCPStreamManager& operator=(const TCPStreamManager& rhs);

    iterator  getByFd(int fd);

    void  addClient(const TCPStream& client);
    void  dropClient(const TCPStream& client);
    void  registerClientEvent(TCPStream& client, const uint32_t event);
    void  unRegisterClientEvent(TCPStream& client, const uint32_t event);

};

#endif
