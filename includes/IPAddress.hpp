/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPAddress.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasaicic <sasaicic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 15:15:57 by sasaicic          #+#    #+#             */
/*   Updated: 2022/11/01 17:01:03 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IP_ADRESS_HPP
#define IP_ADRESS_HPP

#include <string>
#include <sys/socket.h>

class IPAddress
{
  private:
    struct sockaddr  _addr;
    socklen_t        _addrlen;
    bool             _defined;

  public:
    IPAddress();
    IPAddress(const IPAddress& src);
    IPAddress(const struct sockaddr addr, socklen_t addrlen);
    ~IPAddress();

    IPAddress&  operator=(const IPAddress& rhs);

    const std::string toStr() const;
    const sockaddr    addr() const;
    socklen_t   addrlen() const;

    bool  isDefined() const;

};

#endif
