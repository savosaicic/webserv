/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiju-du <abiju-du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 10:39:57 by savosaicic        #+#    #+#             */
/*   Updated: 2022/11/01 17:09:46 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <webserv.hpp>
#include <TCPListener.hpp>
#include <vector>

#include "Config.hpp"

int	main(void) 
{
	TCPListener	tcp;

	std::vector<TCPStream> stream_vect;

	tcp.bind("0.0.0.0", "8080");
	tcp.listen();
	while (1)
		stream_vect = tcp.incoming();

	return (0);
}
