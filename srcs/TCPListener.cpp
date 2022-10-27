#include <TCPListener.hpp>

TCPListener::TCPListener()
{
}

TCPListener::~TCPListener()
{
}

void TCPListener::bind(const char* ip, const char* port)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int yes = 1;

    if (!ip || !port)
        throw std::invalid_argument("TCPListener::bind: ip or port is NULL");

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICHOST;

    if (getaddrinfo(ip, port, &hints, &result) != 0)
        throw std::runtime_error("getaddrinfo() failed");
    
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        int fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd == -1)
            continue;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
            throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
        if (::bind(fd, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            _fd_port_list.push_back(std::make_pair(fd, port));
            break;
        }
        ::close(fd);
    }
    if (!rp)
        throw std::runtime_error("bind() failed");
    freeaddrinfo(result);
}

void TCPListener::listen()
{
    if (_fd_port_list.begin() == _fd_port_list.end())
        throw std::runtime_error("No socket to listen on");
    for (std::vector<std::pair<int, const char*> >::iterator it = _fd_port_list.begin(); it != _fd_port_list.end(); ++it)
    {
        if (::listen(it->first, 10) == -1)
            throw std::runtime_error("listen() failed");
    }
    struct sockaddr address;
    socklen_t address_len;
    int a = ::accept(_fd_port_list.begin()->first, &address, &address_len);
    sleep(1);
    close(a);
}

void TCPListener::incoming()
{

}