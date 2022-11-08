/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <leon.efendiev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:01:15 by lefendie          #+#    #+#             */
/*   Updated: 2022/11/08 10:18:06 by lefendie         ###   ########.fr       */
/* ************************************************************************** */

#ifndef HTTPREQUEST_H
# define HTTPREQUEST_H

#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <list>
#include <map>


typedef std::map<std::string, std::string> headerMap_t;


class HTTPRequest
{
	public:
		enum method_t {GET = 0, POST, DELETE};

	private:
		std::string _raw_request;
		method_t _method;
		std::string _uri;
		std::pair<int, int> _http_version;
		headerMap_t _header_map;
		std::string _body;

		method_t extractMethod(std::string method);
		void extractVersion(std::string httpVersion);
		void extractHeaderMap(std::string request);

	public:


		HTTPRequest(std::string raw_request);
		~HTTPRequest(void);
		HTTPRequest(const HTTPRequest& src);
		HTTPRequest& operator=(const HTTPRequest& rhs);

		const std::string& getHeaderContent(const std::string& header_title);
		bool isComplete();

		method_t	getMethod(void) const; 
		const std::string&	getUri(void) const; 
		const std::pair<int, int>&	getHttpVersion(void) const; 
		const std::string&	getBody(void) const; 
		const std::string&	getRawRequest(void) const; 
		const headerMap_t&	getHeaderMap(void) const; 

};

std::ostream &operator<<(std::ostream &out, const HTTPRequest &stream);

#endif
