/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiju-du <abiju-du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 14:15:54 by sasaicic          #+#    #+#             */
/*   Updated: 2022/08/09 07:47:56 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <sstream>
# include <netdb.h>
# include <string>

# define SA struct sockaddr
# define SA_IN struct sockaddr_in

# define ITOA(x) static_cast< std::ostringstream & >(\
        (std::ostringstream() << std::dec << x)).str().c_str()

class	ws_exception : public std::exception
{
	private:
		std::string	what_message;
	public:
		ws_exception(const std::string &what_msg) : what_message(what_msg) {}
		virtual ~ws_exception() throw() {}
		virtual const char  *what() const throw()
		{
			return (what_message.c_str());
		}
};

const std::string	error_message(const char *item, const char *error);

#endif
