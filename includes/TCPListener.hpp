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

#define MAX_EVENT 10
#define EPOLL_TIMEOUT 500

class TCPListener
{
    int _epfd;
    std::vector<std::pair<int, const char*> > _fd_port_list;

	public:
    TCPListener();
    ~TCPListener();
    // TCPListener operator=(const TCPListener &rhs);
    // TCPListener(const TCPListener &src);

    void bind(const char* ip, const char *port);
    void listen();
		void incoming();
		bool isListener(int fd);
	
};

#endif
