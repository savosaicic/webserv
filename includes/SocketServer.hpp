#ifndef SOCKET_SERVER_HPP
# define SOCKET_SERVER_HPP

class SocketServer
{
protected:
	int	_fd;
	int	_domain;
	int	_type;
	int	_protocol;
	
public:
	SocketServer(int domain, int type, int protocol);
	~SocketServer();

	void	bind(int port);
	void	listen();
	
	void	close();

};

#endif
