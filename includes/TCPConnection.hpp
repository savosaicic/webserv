#ifndef TCP_CONNECTION_HPP
# define TCP_CONNECTION_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include "utils.hpp"

class	TCPConnection
{
	public:
		TCPConnection();
		TCPConnection(int connection_fd, const SA_IN &addr);
		TCPConnection(TCPConnection const &src);
		~TCPConnection();

		void	recv() {}
		void	send() {}
		void	shutdown() {}
		void	close();

	private:
		int		_fd;
		short	_io_events;
		SA_IN	_addr;
};

#endif