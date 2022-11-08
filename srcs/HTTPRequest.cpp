/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <leon.efendiev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:01:14 by lefendie          #+#    #+#             */
/*   Updated: 2022/11/08 10:20:11 by lefendie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include <vector>
#include <utility>
#include <map>
#include <cstdlib>
#include <sstream>
#include <VirtualServer.hpp>

HTTPRequest::HTTPRequest(std::string raw_request)
{
	std::string temp;
	temp = raw_request.substr(0, raw_request.find(" "));
	_method = extractMethod(temp);
	raw_request = raw_request.substr(raw_request.find_first_of(" \t")+1);
	_uri = raw_request.substr(0, raw_request.find(" "));
	raw_request = raw_request.substr(raw_request.find_first_of(" \t")+1);
	extractVersion(raw_request);
	raw_request = raw_request.substr(raw_request.find_first_of("\r\n")+1);
	extractHeaderMap(raw_request);
	return ;
}

void HTTPRequest::extractHeaderMap(std::string request)
{
	std::stringstream ss;
	std::string temp;
	std::pair<std::string, std::string> current;

	ss << request;

	while (std::getline(ss, temp) && !temp.empty() && temp.compare("\r\n"))
	{
		current.first = temp.substr(0, temp.find(":"));
		current.second = temp.substr(temp.find(":")+1, temp.length());
		_header_map.insert(current);
	}
}

HTTPRequest::method_t HTTPRequest::extractMethod(std::string method)
{
	HTTPRequest::method_t ret;

	if (method == "GET")
		ret = GET;
	else if (method == "POST")
		ret = POST;
	else if (method == "DELETE")
		ret = DELETE;
	return (ret);
}

void HTTPRequest::extractVersion(std::string httpVersion)
{
	std::pair<int, int> ret;
	int first;
	int second;

	if (httpVersion.compare(0, 5, "HTTP/") == 0)
	{
		first = ::atoi((httpVersion.substr(5, httpVersion.find("."))).c_str());
		second = ::atoi((httpVersion.substr(7, httpVersion.find("."))).c_str());
		_http_version = std::make_pair(first, second);
	}
	else
		throw std::invalid_argument("Invalid HTTP version");	
}


HTTPRequest::~HTTPRequest(void)
{
	return ;
}

HTTPRequest::HTTPRequest(const HTTPRequest& src)
{
	_method = src._method;
	_uri = src._uri;
	_http_version = src._http_version;
	_body = src._body;
	_raw_request = src._raw_request;
	_header_map = src._header_map;
}

HTTPRequest& HTTPRequest::operator=(const HTTPRequest& rhs)
{

	_method = rhs._method;
	_uri = rhs._uri;
	_http_version = rhs._http_version;
	_body = rhs._body;
	_raw_request = rhs._raw_request;
	_header_map = rhs._header_map;
	return (*this);
}

const std::string& HTTPRequest::getHeaderContent(const std::string& header_title)
{
	headerMap_t::iterator it;

	it = _header_map.find(header_title);
	if (it == _header_map.end())
		throw std::invalid_argument("Request header not found");
	return (it->second);
}

bool HTTPRequest::isComplete()
{
	bool essential_empty = _uri.empty() || _uri.empty() || _http_version.first == 0;
	if (_method == 0 && essential_empty)
		return (false);
	else if (_method == 1 && (essential_empty || _body.empty()))
		return (false);
	else if (_method == 2 && essential_empty)
		return (false);
	return (true);
		
}

HTTPRequest::method_t	HTTPRequest::getMethod(void) const
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

const headerMap_t&	HTTPRequest::getHeaderMap(void) const
{
		return (_header_map);
}

std::ostream &operator<<(std::ostream &out, const HTTPRequest &stream)
{
	out << "method: " << stream.getMethod() << std::endl;
	out << "uri: " << stream.getUri() << std::endl;

	out << "version: " << stream.getHttpVersion().first << '.' << stream.getHttpVersion().second << std::endl;
	out << "Header list:" << std::endl;
	headerMap_t header_map = stream.getHeaderMap();
	for(headerMap_t::iterator it = header_map.begin();it != header_map.end(); it++)
		out << "- " << it->first << ":" << it->second << std::endl;
	return out;
}
