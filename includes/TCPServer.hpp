/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasaicic <sasaicic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 16:48:18 by sasaicic          #+#    #+#             */
/*   Updated: 2022/08/05 16:48:20 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

# include <vector>

class	TCPConnection
{
	public:
		TCPConnection();
		TCPConnection(TCPConnection const &src);
		~TCPConnection();

		void	recv();
		void	send();
		void	shutdown();
		void	close();

	private:
		int		_fd;
		short	_io_events;
};

class	TCPServer
{
	public:
		TCPServer();
		TCPServer(TCPServer const &src);
		~TCPServer();

		void	start();
		void	accept();
		void	stop();

	private:
		std::vector<TCPConnection>	_connection;
};

#endif
