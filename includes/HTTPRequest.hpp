#ifndef HTTPREQUEST_H
# define HTTPREQUEST_H

#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <list>
#include <map>
#include <HTTPVersion.hpp>


#define CRLF "\r\n"

typedef std::map<std::string, std::string> headerMap_t;
static const std::string Empty = "";

enum status_t {METHOD = 0, URI, VERSION, HEADER, BODY};

class HTTPRequest
{
	private:
		enum status_t {METHOD = 0, URI, VERSION, HEADER, BODY, END};
	public:
		enum method_t {GET = 0, POST, DELETE};

	private:
		HTTPVersion	_http_version;
		headerMap_t	_header_map;
		std::string	_uri;
		std::string	_body;
		method_t		_method;
		status_t		_to_parse;
		std::string	_buffer;
		bool				_is_valid;
		bool				_is_complete;

		void	extractMethod(const std::string& method);
		const	std::string getNextWord(std::string& line) const;
		const	std::string& eraseFirstWord(std::string& line) const;
		const	std::string getNextLine(std::string& line) const;
		const	std::string& eraseCurrentLine(std::string& line) const;
		void	extractVersion(std::string httpVersion);
		int		extractHeaderMap(std::string &request);

		void parseRequestLine(std::string& raw_request);
		void parseRequestHeader(std::string& raw_request);
		void parseRequestBody(std::string& raw_request);

		bool	requestLineParsable(const std::string& raw_request) const;
		bool	headerBodyParsable(const std::string& raw_request) const;

	public:
		HTTPRequest(void);
		~HTTPRequest(void);
		HTTPRequest(const HTTPRequest& src);
		HTTPRequest& operator=(const HTTPRequest& rhs);

		void parseRequest(std::string raw_request);

		const std::string&	getHeaderContent(const std::string& header_title) const;
		bool								isValid()const;
		bool								isComplete() const;

		std::string	getMethodString(void) const;
		method_t		getMethod(void) const; 
		const				std::string&	getUri(void) const; 
		const 			HTTPVersion&	getHTTPVersion(void) const; 
		const 			std::string&	getBody(void) const; 
		const 			headerMap_t&	getHeaderMap(void) const; 

		void		addHeader(const std::string& key, const std::string& value);
		void	setBody(const std::string& body);
		void	setHTTPVersion(const std::string& http_version);
		void 	setHTTPVersion(const int prior, const int minor);
		void 	setHTTPVersion(const HTTPVersion& http_version);
		void 	setUri(const std::string& uri);
		void 	setMethod(const HTTPRequest::method_t& method);

};

std::ostream &operator<<(std::ostream &out, const HTTPRequest &stream);

#endif
