/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lefendie <leon.efendiev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:01:14 by lefendie          #+#    #+#             */
/*   Updated: 2022/11/07 11:17:34 by lefendie         ###   ########.fr       */
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
	raw_request = raw_request.substr(raw_request.find_first_of("\n")+1);
	extractHeaderMap(raw_request);
	return ;
}

void HTTPRequest::extractHeaderMap(std::string request)
{
	std::stringstream ss;
	std::string temp;
	std::pair<std::string, std::string> current;

	ss << request;

	while (std::getline(ss, temp) && !temp.empty() && temp.compare("\n"))
	{
		current.first = temp.substr(0, temp.find(":"));
		current.second = temp.substr(temp.find(":")+1, temp.length());
		_header_map.insert(current);
	}
}

method_t HTTPRequest::extractMethod(std::string method)
{
	method_t ret;

	if (method == "GET")
		ret = 0;
	else if (method == "POST")
		ret = 1;
	else if (method == "DELETE")
		ret = 2;
	return (ret);
}

void HTTPRequest::extractVersion(std::string httpVersion)
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
	_header_map = inst._header_map;
}

HTTPRequest& HTTPRequest::operator=(const HTTPRequest& inst)
{

	_method = inst._method;
	_uri = inst._uri;
	_http_version = inst._http_version;
	_body = inst._body;
	_raw_request = inst._raw_request;
	_header_map = inst._header_map;
	return (*this);
}

std::map<std::string, std::string>::iterator HTTPRequest::getHeaderContent(const std::string& header_title)
{
	std::map<std::string, std::string>::iterator it;

	it = _header_map.find(header_title);
	return (it);
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

const std::map<std::string, std::string>&	HTTPRequest::getHeaderMap(void) const
{
		return (_header_map);
}

std::ostream &operator<<(std::ostream &out, const HTTPRequest &stream)
{
	out << "raw_request: " << stream.getRawRequest() << std::endl;
	out << "method: " << stream.getMethod() << std::endl;
	out << "uri: " << stream.getUri() << std::endl;

	out << "version: " << stream.getHttpVersion().first << '.' << stream.getHttpVersion().second << std::endl;
	out << "Header list:" << std::endl;
	std::map<std::string, std::string> header_map = stream.getHeaderMap();
	for(std::map<std::string, std::string>::iterator it = header_map.begin();it != header_map.end(); it++)
		out << "- " << it->first << ":" << it->second << std::endl;
	return out;
}
