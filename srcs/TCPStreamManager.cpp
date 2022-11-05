#include "TCPStreamManager.hpp"
#include "TCPListener.hpp"
#include "TCPStream.hpp"
#include <algorithm>
#include <utility>

TCPStreamManager::TCPStreamManager(int& epfd, TCPListener& listener)
  : _epfd(epfd), _listener(listener)
{
}

TCPStreamManager::TCPStreamManager(const TCPStreamManager& src)
  : _epfd(src._epfd), _listener(src._listener),
  _connected_clients(src._connected_clients)
{
}

TCPStreamManager::~TCPStreamManager()
{
}

TCPStreamManager& TCPStreamManager::operator=(const TCPStreamManager& rhs)
{
  if (this != &rhs)
  {
    _epfd = rhs._epfd;
    _listener = rhs._listener;
    _connected_clients = rhs._connected_clients;
  }
  return *this;
}

TCPStreamManager::iterator  TCPStreamManager::getByFd(int fd)
{
  return _connected_clients.find(fd);
}

void  TCPStreamManager::addClient(const TCPStream& client)
{
  _connected_clients.insert(std::make_pair(client.getFd(), client));
}

void  TCPStreamManager::dropClient(const TCPStream& client)
{
  TCPStreamManager::iterator it = _connected_clients.find(client.getFd());
  if (it != _connected_clients.end())
  {
    epoll_ctl(_epfd, EPOLL_CTL_DEL, it->second.getFd(), NULL);
    close(it->second.getFd());
    _connected_clients.erase(it);
  }
}

void  TCPStreamManager::registerClientEvent(TCPStream& client, const uint32_t event)
{
  TCPStreamManager::iterator it =_connected_clients.find(client.getFd());
  if (it != _connected_clients.end())
  {
    struct epoll_event  events;
    memset(&events, 0, sizeof(events));

    it->second.registerEvent(event);

    events.data.fd = it->second.getFd();
    events.events = it->second.getRegisteredEvents();
    epoll_ctl(_epfd, EPOLL_CTL_MOD, it->second.getFd(), &events);
  }
}

void  TCPStreamManager::unRegisterClientEvent(TCPStream& client, const uint32_t event)
{
  TCPStreamManager::iterator it = _connected_clients.find(client.getFd());
  if (it != _connected_clients.end())
  {
    struct epoll_event  events;
    memset(&events, 0, sizeof(events));

    it->second.unRegisterEvent(event);

    events.data.fd = it->second.getFd();
    events.events = it->second.getRegisteredEvents();
    epoll_ctl(_epfd, EPOLL_CTL_MOD, it->second.getFd(), &events);
  }
}
