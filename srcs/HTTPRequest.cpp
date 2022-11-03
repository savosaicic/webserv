/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <leon.efendiev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:01:14 by lefendie          #+#    #+#             */
/*   Updated: 2022/11/03 09:05:08 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include <vector>
#include <utility>
#include <map>

HTTPRequest::HTTPRequest(method_t _method, std::string _uri, std::pair<int, int> _http_version): _method(_method),  _uri(_uri),  _http_version(_http_version)
{
	return ;
}

HTTPRequest::~HTTPRequest(void)
{
	return ;
}

HTTPRequest::HTTPRequest(const HTTPRequest& inst)
{
	_method = inst._method;
	_uri = inst._uri;
	_http_version = inst._http_version;
	_body = inst._body;
	_raw_request = inst._raw_request;
	_header_list = inst._header_list;
}

HTTPRequest& HTTPRequest::operator=(const HTTPRequest& inst)
{

	_method = inst._method;
	_uri = inst._uri;
	_http_version = inst._http_version;
	_body = inst._body;
	_raw_request = inst._raw_request;
	_header_list = inst._header_list;
	return (*this);
}

method_t	HTTPRequest::getMethod(void) const
{
		return (_method);
}

const std::string&	HTTPRequest::getUri(void) const
{
		return (_uri);
}

const std::pair<int, int>&	HTTPRequest::getHttpVersion(void) const
{
		return (_http_version);
}

const std::string&	HTTPRequest::getBody(void) const
{
		return (_body);
}

const std::string&	HTTPRequest::getRawRequest(void) const
{
		return (_raw_request);
}

const std::map<std::string, std::string>&	HTTPRequest::getHeader_list(void) const
{
		return (_header_list);
}

void	HTTPRequest::setMethod(method_t method)
{
		_method = method;
}

void	HTTPRequest::setUri(std::string uri)
{
		_uri = uri;
}

void	HTTPRequest::setHttpVersion(std::pair<int, int> http_version)
{
		_http_version = http_version;
}

void	HTTPRequest::setBody(std::string body)
{
		_body = body;
}

void	HTTPRequest::setRawRequest(std::string raw_request)
{
		_raw_request = raw_request;
}

void	HTTPRequest::setHeaderList(std::map<std::string, std::string> header_list)
{
		_header_list = header_list;
}
