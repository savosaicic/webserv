#include "HTTPRequest.hpp"
#include <vector>
#include <utility>
#include <map>
#include <cstdlib>
#include <sstream>

HTTPRequest::HTTPRequest(std::string raw_request)
{
	if (raw_request.find_first_not_of("	 "))
		_method = INVALID;
	else
		_method = extractMethod(getNextWord(raw_request));
	if (_method != INVALID)
	{
		eraseFirstWord(raw_request);
		_uri = getNextWord(raw_request);
		eraseFirstWord(raw_request);
		extractVersion(getNextWord(raw_request));
		if (raw_request.size() > 1 && raw_request.find_first_of("\r\n") != std::string::npos)
		{
			if (!raw_request.compare(raw_request.find_first_of("\r\n"), 2, "\r\n"))
			{
				raw_request = getNextLine(raw_request);
				extractHeaderMap(raw_request);
			}
			if (_method == POST)
				_body = raw_request;
		}
	}
	return ;
}

const std::string HTTPRequest::getNextWord(std::string& line)
{
	std::string word;
	line.erase(0, line.find_first_not_of(" "));
	if (line.find_first_of(" "))
		word = line.substr(0, line.find_first_of(" "));
	return word;
}

const std::string HTTPRequest::getNextLine(std::string& line)
{
	std::string next_line;
	if (line.find_first_of("\r\n"))
		next_line = line.substr(line.find_first_of("\r\n")+2);
	return next_line;
}

const std::string& HTTPRequest::eraseFirstWord(std::string& line)
{
	if (line.find_first_of("		"))
		line.erase(0, line.find_first_of(" ")+1);
	return line;
}

void HTTPRequest::extractHeaderMap(std::string &request)
{
	std::stringstream ss;
	std::string temp = " ";
	std::pair<std::string, std::string> current;

	ss << request;

	while (std::getline(ss, temp) && temp != "\r")
	{
		current.first = temp.substr(0, temp.find(":"));
		current.second = temp.substr(temp.find(":")+1, temp.length());
		_header_map.insert(current);
	}
	std::getline(ss, request);
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
	else
		ret = INVALID;
	return (ret);
}

void HTTPRequest::extractVersion(std::string http_version)
{
	std::pair<int, int> ret;
	int first;
	int second;
	std::string temp;

	first = -1;
	second = -1;
	http_version.erase(0, http_version.find_first_not_of("	"));
	if (http_version.size() > 4)
	{
		if (http_version.compare(0, 5, "HTTP/") == 0)
		{
			temp = (http_version.substr(5, http_version.find(".") - 5));
			if (!temp.empty() && temp.find_first_not_of("0123456789") == std::string::npos)
			{
				first = ::atoi(temp.c_str());
				http_version.erase(0, 6 + temp.size());
				temp = http_version.substr(0, http_version.find_first_not_of("0123456789"));
				if (!temp.empty())
					second = ::atoi(http_version.c_str());
			}
		}
	}
	_http_version = std::make_pair(first, second);
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
	headerMap_t::const_iterator it = _header_map.find(header_title);
	if (it == _header_map.end())
		return Empty;
	return (it->second);
}

bool HTTPRequest::isValid() 
{
	std::map<std::string, std::string>::const_iterator it;
	if (_method == INVALID)
		return (false);
	if (_http_version.first == -1 || _http_version.second == -1)
		return (false);
	it = _header_map.find("");
	if (it != _header_map.end())
		return (false);
	if (_uri.empty())
		return (false);
	return (true);
}

bool HTTPRequest::isComplete() const
{
	bool essential_empty = _uri.empty() || _uri.empty() || _http_version.first == 0;
	if (_method == GET && essential_empty)
		return (false);
	else if (_method == POST && (essential_empty || _body.empty()))
		return (false);
	else if (_method == DELETE && essential_empty)
		return (false);
	else if (_method == INVALID)
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

const std::pair<int, int>&	HTTPRequest::getHTTPVersion(void) const
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

void HTTPRequest::setMethod(HTTPRequest::method_t method)
{
	_method = method;
} 

void HTTPRequest::setUri(std::string uri)
{
	_uri = uri;
}

void HTTPRequest::setHTTPVersion(std::pair<int, int> http_version)
{
	_http_version = http_version;
}

void HTTPRequest::setHTTPVersion(int prior, int minor)
{
	if (prior > 0)
		_http_version.first = prior;
	else
		_http_version.first = -1;
	if (minor >= 0)
		_http_version.second = minor;
	else
		_http_version.second = -1;
} 

void HTTPRequest::addHeader(std::string header, std::string value)
{
	std::pair<std::string, std::string> current;

	current.first = "";
	if (current.first.find_first_of("	 ") != std::string::npos)
		current.first = header;
	current.second = value;
	_header_map.insert(current);
} 

void HTTPRequest::setBody(std::string body)
{
	_body = body;
}

void HTTPRequest::setHTTPVersion(std::string http_version)
{
	extractVersion(http_version);
}

std::ostream &operator<<(std::ostream &out, const HTTPRequest &stream)
{
	if (stream.isComplete())
	{
		out << "method: " << stream.getMethod() << std::endl;
		out << "uri: " << stream.getUri() << std::endl;

		out << "version: " << stream.getHTTPVersion().first << '.' << stream.getHTTPVersion().second << std::endl;
		out << "Header list:" << std::endl;
		headerMap_t header_map = stream.getHeaderMap();
		for(headerMap_t::const_iterator it = header_map.begin();it != header_map.end(); it++)
			out << "- " << it->first << ":" << it->second << std::endl;
		out << "Body:" <<  stream.getBody() << std::endl;
	}
	return out;
}
