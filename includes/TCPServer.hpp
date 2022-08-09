/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasaicic <sasaicic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 16:48:18 by sasaicic          #+#    #+#             */
/*   Updated: 2022/08/09 08:49:04 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

# include "SocketServer.hpp"
# include "TCPConnection.hpp"
# include "utils.hpp"
# include <vector>

class	TCPServer : public SocketServer
{
	public:
		TCPServer();
		TCPServer(TCPServer const &src);
		~TCPServer();

		void	listen(int port, SA_IN addr);
		void	accept();
		void	close();

	private:
		std::vector<TCPConnection>	_connections;
};

#endif
