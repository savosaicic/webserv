#ifndef TCP_LISTENER_HPP
#define TCP_LISTENER_HPP

#include <vector>
#include <utility>
#include <stdint.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdexcept>
#include <unistd.h>
#include <cstdio>
#include <sys/epoll.h>
#include <TCPStream.hpp>
#include "TCPStreamManager.hpp"

#define MAX_EVENT 10
#define EPOLL_TIMEOUT 500

class TCPListener
{
  private:
    int _epfd;
    std::vector<std::pair<int, const char*> > _fd_port_list;
    TCPStreamManager  _streams_manager;

	public:
    TCPListener();
    TCPListener(const TCPListener &src);
    ~TCPListener();

    TCPListener operator=(const TCPListener &rhs);

    void bind(const char* ip, const char *port);
    void listen();
    std::vector<TCPStreamManager::iterator> incoming();
		bool isListener(int fd);

    void  dropClient(const TCPStream& client);
    void  registerClientEvent(TCPStream& client, const uint32_t event);
    void  unRegisterClientEvent(TCPStream& client, const uint32_t event);

};

#endif
