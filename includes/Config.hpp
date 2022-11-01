/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasaicic <sasaicic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 16:26:04 by sasaicic          #+#    #+#             */
/*   Updated: 2022/11/01 09:28:51 by sasaicic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <string>
#include "VirtualServer.hpp"

class Config
{
  private:
    std::vector<VirtualServer>  _server_list;

  public:
    Config();
    Config(const Config& src);
    ~Config();

    Config& operator=(const Config& rhs);

    void  parseConfigFile(const char* filename);

    // This method will return a server with a given :
    // - Listening address (ip+port)
    // - a server_name (From http request header `Host`)
    const VirtualServer&  getServer(const char* port,
        const std::string& hostname) const;
};

#endif
