#include "HTTPRequest.hpp"
#include <vector>
#include <utility>
#include <map>
#include <cstdlib>
#include <sstream>
#include <sys/types.h>
#include <http_utils.hpp>
#include <HTTPVersion.hpp>

HTTPRequest::HTTPRequest(void) : _is_valid(true), _is_complete(false),
  _to_parse(METHOD), _content_length(0)
{
}

HTTPRequest::HTTPRequest(unsigned int max_body_size) : _is_valid(true), _is_complete(false),
  _to_parse(METHOD), _max_body_size(max_body_size)
{
}

int HTTPRequest::extractHeaderMap(std::string &line)
{
	std::string key;
	std::string value;

	key = line.substr(0, line.find(":"));
	value = line.substr(line.find(":")+1, line.find_first_of("\r\n"));
	if (value.find_last_of("\r") != std::string::npos)
	{
		value.erase(0, value.find_first_not_of(" "));
		value.erase(value.find_last_of("\r"), value.length());
	}
	addHeader(key, value);
	return (0);
}

bool	HTTPRequest::requestLineParsable(const std::string& raw_request) const
{
 return (_to_parse == METHOD || _to_parse == URI) && raw_request.find_first_of(" ") != std::string::npos;
}

bool	HTTPRequest::headerParsable(const std::string& raw_request) const
{
 return (_to_parse == VERSION || _to_parse == HEADER) && raw_request.find_first_of("\n") != std::string::npos;
}

bool	HTTPRequest::bodyParsable(void) const
{
 return _body.size() <= _content_length && _to_parse == BODY;
}

void HTTPRequest::parseRequestHeader(std::string& raw_request)
{
	std::stringstream ss;
	std::string line;
	ss << raw_request;
	if (raw_request.find_first_of("\n") != std::string::npos)
	{
		while (std::getline(ss, line) && _is_complete == false && _is_valid && !line.empty())
		{
			if (line == "\r")
			{
				if (_method == POST)
				{
					_to_parse = BODY;
					_content_length = atoi(getHeaderContent("Content-Length").c_str());
					eraseCurrentLine(raw_request);
					break;
				}
				else
					_is_complete = true;
			}
			else
			{
				extractHeaderMap(line);
				eraseCurrentLine(raw_request);
			}
		}
	}
}

void HTTPRequest::parseRequestBody(std::string& raw_request)
{

	_body += raw_request;
	if (_body.size() >= _content_length)
	{
		_is_complete = true;
		_body.erase(_content_length, std::string::npos);
	}
}

void HTTPRequest::parseRequestLine(std::string& raw_request)
{
	raw_request.erase(0, raw_request.find_first_not_of(" "));
	while (_is_valid && _to_parse != HEADER && !raw_request.empty()
				&& (requestLineParsable(raw_request) || headerParsable(raw_request) || bodyParsable())
				)
	{
		switch((int)_to_parse)
		{
			case METHOD:
				extractMethod(getNextWord(raw_request));
				eraseFirstWord(raw_request);
				_to_parse = URI;
				break;
			case URI:
				_uri = getNextWord(raw_request);
				eraseFirstWord(raw_request);
				_to_parse = VERSION;
				_buffer = raw_request;
				break;
			case VERSION:
				extractVersion(getNextWord(raw_request));
				eraseFirstWord(raw_request);
				//raw_request = getNextLine(raw_request);
				_to_parse = HEADER;
				break;
		}
	}
}

void	HTTPRequest::parseRequest(std::string raw_request)
{
	raw_request = _buffer + raw_request;
	while (_is_valid && !_is_complete
			&& (requestLineParsable(raw_request) || headerParsable(raw_request) || bodyParsable())
			&& !raw_request.empty())
	{
		switch ((int) _to_parse)
		{
			case HEADER:
				parseRequestHeader(raw_request);
				break;
			case BODY:
				if (_content_length == 0)
					_is_complete = true;
				else if (_content_length > _max_body_size)
				{
					_is_valid = false;
					_error_code = status_code::http_payload_too_large;
				}
				parseRequestBody(raw_request);
				break;
			default:
				parseRequestLine(raw_request);
				break;
		}
	}
	_buffer = raw_request;
}

int HTTPRequest::errorCode() const
{
	if (_error_code == status_code::http_ok)
		throw std::logic_error("No error");
	return (_error_code);
}

const std::string HTTPRequest::getNextWord(std::string& line) const
{
	std::string word;
	line.erase(0, line.find_first_not_of(" \r\n"));
	word = line.substr(0, line.find_first_of(" \r\n"));
	return word;
}

const std::string HTTPRequest::getNextLine(std::string& line) const 
{
	std::string next_line;
	if (line.find_first_of("\r\n") != std::string::npos)
		next_line = line.substr(line.find_first_of("\r\n")+2);
	return next_line;
}

const std::string& HTTPRequest::eraseFirstWord(std::string& line) const 
{
	if (line.find_first_of(" \r\n"))
  {
		line.erase(0, line.find_first_of(" \r\n"));
		line.erase(0, line.find_first_not_of(" \r\n"));
  }
	return line;
}

const std::string& HTTPRequest::eraseCurrentLine(std::string& line) const 
{
	if (line.find_first_of("\n"))
		line.erase(0, line.find_first_of("\n")+1);
	return line;
}


void HTTPRequest::extractMethod(const std::string& method)
{
	if (method == "GET")
		_method = GET;
	else if (method == "POST")
		_method = POST;
	else if (method == "DELETE")
		_method = DELETE;
	else
	{
		_is_valid = false;
		_error_code = status_code::http_not_implemented;
	}
}

void HTTPRequest::extractVersion(std::string http_version)
{
	int major;
	int minor;
	std::string temp;

	major = -1;
	minor = -1;
	http_version.erase(0, http_version.find_first_not_of("	"));
	if (http_version.size() > 4)
	{
		if (http_version.compare(0, 5, "HTTP/") == 0)
		{
			temp = (http_version.substr(5, http_version.find(".") - 5));
			if (!temp.empty() && temp.find_first_not_of("0123456789") == std::string::npos)
			{
				major = ::atoi(temp.c_str());
				http_version.erase(0, 6 + temp.size());
				temp = http_version.substr(0, http_version.find_first_not_of("0123456789"));
				if (!temp.empty())
					minor = ::atoi(http_version.c_str());
			}
		}
	}
	if (major == -1 || minor == -1)
	{
		_is_valid = false;
		_error_code = status_code::http_version_not_supported;
	}
	else
		_http_version = HTTPVersion(major, minor);
}

HTTPRequest::~HTTPRequest(void)
{
	return ;
}

HTTPRequest::HTTPRequest(const HTTPRequest& src)
  : _method(src._method), _uri(src._uri), _http_version(src._http_version),
    _body(src._body), _header_map(src._header_map), _is_valid(src._is_valid),
    _is_complete(src._is_complete), _to_parse(src._to_parse),
		_buffer(src._buffer), _content_length(src._content_length),
		_error_code(src._error_code),
		_max_body_size(src._max_body_size)
{}

HTTPRequest& HTTPRequest::operator=(const HTTPRequest& rhs)
{
  if (this != &rhs)
  {
    _method = rhs._method;
    _uri = rhs._uri;
    _http_version = rhs._http_version;
    _body = rhs._body;
    _header_map = rhs._header_map;
    _is_valid = rhs._is_valid;
    _is_complete = rhs._is_complete;
		_to_parse = rhs._to_parse;
		_buffer = rhs._buffer;
		_content_length = rhs._content_length;
		_error_code = rhs._error_code;
		_max_body_size = rhs._max_body_size;
  }
	return (*this);
}

const std::string& HTTPRequest::getHeaderContent(const std::string& header_title) const
{
	headerMap_t::const_iterator it = _header_map.find(header_title);
	if (it == _header_map.end())
		return Empty;
	return (it->second);
}

HTTPRequest::method_t	HTTPRequest::getMethod(void) const
{
		return (_method);
}

std::string	HTTPRequest::getMethodString(void) const
{
	if (_method == GET)
		return ("GET");
	else if (_method == POST)
		return ("POST");
	else if (_method == DELETE)
		return ("DELETE");
	return ("");	
}

const std::string&	HTTPRequest::getUri(void) const
{
		return (_uri);
}

const HTTPVersion&	HTTPRequest::getHTTPVersion(void) const
{
		return (_http_version);
}

const std::string&	HTTPRequest::getBody(void) const
{
		return (_body);
}

const headerMap_t&	HTTPRequest::getHeaderMap(void) const
{
		return (_header_map);
}

void HTTPRequest::setMethod(const HTTPRequest::method_t& method)
{
	_method = method;
} 

void HTTPRequest::setUri(const std::string& uri)
{
	_uri = uri;
}

void HTTPRequest::setHTTPVersion(const HTTPVersion& http_version)
{
	_http_version = http_version;
}

void HTTPRequest::setHTTPVersion(const int major, const int minor)
{
	_http_version.setMajor(major);
	_http_version.setMinor(minor);
} 

void HTTPRequest::addHeader(const std::string& key, const std::string& value)
{
	size_t i = 0;
	if (key.empty())
		_is_valid = false;
	while (_is_valid && i < key.size())
	{
		if (key[i] < 33 || key[i] > 126 || key[i] == ':')
			_is_valid = false;
		i++;
	}
	if (_is_valid == false)
		_error_code = status_code::http_bad_request;
	_header_map.insert(std::make_pair(key, value));	
} 

void HTTPRequest::setBody(const std::string& body)
{
	_body = body;
}

void HTTPRequest::setHTTPVersion(const std::string& http_version)
{
	extractVersion(http_version);
}

std::ostream &operator<<(std::ostream &out, const HTTPRequest &stream)
{
	if (stream.isValid() == false)
	{
		out << "Invalid Request: " << stream.errorCode() << '\n';

	}
	if (stream.isComplete())
	{
		out << "method: " << stream.getMethod() << std::endl;
		out << "uri: " << stream.getUri() << std::endl;

		out << "version: " << stream.getHTTPVersion().getMajor() << '.' << stream.getHTTPVersion().getMinor() << std::endl;
		out << "Header list:" << std::endl;
		headerMap_t header_map = stream.getHeaderMap();
		for(headerMap_t::const_iterator it = header_map.begin();it != header_map.end(); it++)
			out << "- " << it->first << ":" << it->second << std::endl;
		out << "Body:" <<  stream.getBody() << std::endl;
	}
	else
		out << "Incomplete Request" << '\n';
	return out;
}

bool HTTPRequest::isValid(void) const
{
	return (_is_valid);
}
bool HTTPRequest::isComplete(void) const
{
	return (_is_complete);
}
