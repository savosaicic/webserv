/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <leon.efendiev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:01:15 by lefendie          #+#    #+#             */
/*   Updated: 2022/11/03 09:05:34 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_H
# define HTTPREQUEST_H

#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <list>
#include <map>

typedef int method_t ;

class HTTPRequest
{
	private:
		method_t _method;
		std::string _uri;
		std::pair<int, int> _http_version;
		std::string _body;
		std::string _raw_request;
		std::map<std::string, std::string> _header_list;
	public:
		HTTPRequest(method_t _method, std::string _uri, std::pair<int, int> _http_version);
		~HTTPRequest(void);
		HTTPRequest(const HTTPRequest& inst);
		HTTPRequest& operator=(const HTTPRequest& inst);
		method_t	getMethod(void) const; 
		void	setMethod(method_t method);
		const std::string&	getUri(void) const; 
		void	setUri(std::string uri);
		const std::pair<int, int>&	getHttpVersion(void) const; 
		void	setHttpVersion(std::pair<int, int> http_version);
		const std::string&	getBody(void) const; 
		void	setBody(std::string body);
		const std::string&	getRawRequest(void) const; 
		void	setRawRequest(std::string raw_request);
		const std::map<std::string, std::string>&	getHeader_list(void) const; 
		void	setHeaderList(std::map<std::string, std::string> header_list);
};
#endif
