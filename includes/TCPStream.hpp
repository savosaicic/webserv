#ifndef TCPSTREAM_H
# define TCPSTREAM_H

#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include "IPAddress.hpp"

class TCPStream
{
	private:
		int _fd;
		IPAddress _ip;
		uint32_t _events;
		uint32_t _triggered_events;
		std::string _in_msg;
		std::string _out_msg;
		int _port;
	
	public:
		TCPStream();
		TCPStream(const int fd, struct sockaddr addr,
				const socklen_t addrlen, const uint32_t events);
		~TCPStream();
		TCPStream& operator=(const TCPStream& rhs);
		TCPStream(const TCPStream& src);
		
		void registerEvent(const uint32_t events);
		void unRegisterEvent(const uint32_t events);
		void resetEvents();
		void setTriggeredEvents(const uint32_t events);

		int getFd() const;
		const IPAddress& getIPAddr() const;
		bool  canRead() const;
    bool  canWrite() const;
    uint32_t getRegisteredEvents() const;
		const std::string& getInMessage() const;
		const std::string& getOutMessage() const;

    void  setOutMessage(const std::string& str);
    void  clearInMessage();
    void  clearOutMessage();

    int read();
    int write(const std::string& str);

    bool  operator==(const TCPStream& rhs);
};

std::ostream &operator<<(std::ostream &out, const TCPStream &stream);


#endif
