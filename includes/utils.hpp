/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiju-du <abiju-du@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/08 14:15:54 by sasaicic          #+#    #+#             */
/*   Updated: 2022/08/08 16:54:18 by abiju-du         ###   ########.fr       */
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

#endif