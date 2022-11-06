/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <leon.efendiev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:01:14 by lefendie          #+#    #+#             */
/*   Updated: 2022/11/05 10:11:32 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include <vector>
#include <utility>
#include <map>
#include <cstdlib>

HTTPRequest::HTTPRequest(std::string raw_request)
{
	std::string temp;
	temp = raw_request.substr(0, raw_request.find(" "));
	_method = extractMethod(temp);

	raw_request.erase(raw_request.find(" ")); 
	_uri = raw_request.substr(0, raw_request.find(" "));
	raw_request.erase(raw_request.find(" ")); 

	extractHTTPVersion(raw_request);
	temp = raw_request.substr(0, raw_request.find("\r\n"));
	return ;
}

method_t HTTPRequest::extractMethod(std::string method)
{
	int ret;

	if (method == "GET")
		ret = GET;
	else if (method == "POST")
		ret = POST;
	else if (method == "DELETE")
		ret = DELETE;
	return ((method_t)ret);
}

void HTTPRequest::extractHTTPVersion(std::string httpVersion)
{
	std::pair<int, int> ret;
	int i = 0;
	int first;
	int second;

	while (!isdigit(httpVersion.at(i)))
		i++;
	first = ::atoi((httpVersion.substr(i, httpVersion.find("."))).c_str());

	while (!isdigit(httpVersion.at(i)))
		i++;
	second = ::atoi((httpVersion.substr(i, httpVersion.find("."))).c_str());
	_http_version = std::make_pair(first, second);
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

std::map<std::string, std::string>::iterator HTTPRequest::getHeaderContent(const std::string& header_title)
{
	std::map<std::string, std::string>::iterator it;

	it = _header_list.find(header_title);
	return (it);
}

bool isComplete()
{
		
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
