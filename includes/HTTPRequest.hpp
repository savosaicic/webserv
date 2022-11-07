/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <leon.efendiev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:01:15 by lefendie          #+#    #+#             */
/*   Updated: 2022/11/07 11:11:22 by lefendie         ###   ########.fr       */
/* ************************************************************************** */

#ifndef HTTPREQUEST_H
# define HTTPREQUEST_H

#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <list>
#include <map>
#include <VirtualServer.hpp>

typedef int method_t ;

class HTTPRequest
{
	private:
		std::string _raw_request;
		method_t _method;
		std::string _uri;
		std::pair<int, int> _http_version;
		std::map<std::string, std::string> _header_map;
		std::string _body;

		method_t extractMethod(std::string method);
		void extractVersion(std::string httpVersion);
		void extractHeaderMap(std::string request);
	public:
		HTTPRequest(std::string raw_request);
		~HTTPRequest(void);
		HTTPRequest(const HTTPRequest& inst);
		HTTPRequest& operator=(const HTTPRequest& inst);

		std::map<std::string, std::string>::iterator getHeaderContent(const std::string& header_title);
		bool isComplete();

		method_t	getMethod(void) const; 
		const std::string&	getUri(void) const; 
		const std::pair<int, int>&	getHttpVersion(void) const; 
		const std::string&	getBody(void) const; 
		const std::string&	getRawRequest(void) const; 
		const std::map<std::string, std::string>&	getHeaderMap(void) const; 

};

std::ostream &operator<<(std::ostream &out, const HTTPRequest &stream);

#endif
