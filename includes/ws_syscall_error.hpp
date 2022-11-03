/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ws_syscall_error.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasaicic <sasaicic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 11:46:08 by sasaicic          #+#    #+#             */
/*   Updated: 2022/11/03 11:49:18 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include <string>
#include <string.h>
#include <errno.h>

class ws_syscall_error : public std::exception
{
  private:
    const std::string str;

  public:
    ws_syscall_error(const std::string & message)
      : str(message + ": " + strerror(errno)) {}

    virtual const char* what() const throw () { return str.c_str(); }
};
