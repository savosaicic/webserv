#include "TCPStreamManager.hpp"
#include "TCPListener.hpp"
#include "TCPStream.hpp"
#include <algorithm>

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

std::vector<TCPStream>::iterator  TCPStreamManager::getByFd(int fd)
{
  std::vector<TCPStream>::iterator it(_connected_clients.begin());
  for (; it != _connected_clients.end(); ++it)
  {
    if (it->getFd() == fd)
      break;
  }
  return it;
}

void  TCPStreamManager::addClient(const TCPStream& client)
{
  _connected_clients.push_back(client);
}

void  TCPStreamManager::dropClient(const TCPStream& client)
{
  typedef std::vector<TCPStream>::iterator  stream_iterator;

  stream_iterator it = std::find(_connected_clients.begin(),
      _connected_clients.end(), client);
  if (it != _connected_clients.end())
  {
    epoll_ctl(_epfd, EPOLL_CTL_DEL, it->getFd(), NULL);
    close(it->getFd());
    _connected_clients.erase(it);
  }
}

void  TCPStreamManager::registerClientEvent(TCPStream& client, const uint32_t event)
{
  TCPStreamManager::iterator it = std::find(_connected_clients.begin(),
      _connected_clients.end(), client);
  if (it != _connected_clients.end())
  {
    struct epoll_event  events;
    memset(&events, 0, sizeof(events));

    it->registerEvent(event);

    events.data.fd = it->getFd();
    events.events = it->getRegisteredEvents();
    epoll_ctl(_epfd, EPOLL_CTL_MOD, it->getFd(), &events);
  }
}

void  TCPStreamManager::unRegisterClientEvent(TCPStream& client, const uint32_t event)
{
  TCPStreamManager::iterator it = std::find(_connected_clients.begin(),
      _connected_clients.end(), client);
  if (it != _connected_clients.end())
  {
    struct epoll_event  events;
    memset(&events, 0, sizeof(events));

    it->unRegisterEvent(event);

    events.data.fd = it->getFd();
    events.events = it->getRegisteredEvents();
    epoll_ctl(_epfd, EPOLL_CTL_MOD, it->getFd(), &events);
  }
}
