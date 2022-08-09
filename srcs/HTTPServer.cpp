/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasaicic <sasaicic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 10:48:45 by sasaicic          #+#    #+#             */
/*   Updated: 2022/08/09 08:07:02 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServer.hpp"
#include "TCPServer.hpp"
#include <iostream>

HTTPServer::HTTPServer()
{
}

HTTPServer::~HTTPServer()
{
}

void	HTTPServer::start()
{
	this->listen(_port, _serveraddr);
	
	this->accept();
}	
