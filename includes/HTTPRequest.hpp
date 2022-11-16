#ifndef HTTPREQUEST_H
# define HTTPREQUEST_H

#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <list>
#include <map>


typedef std::map<std::string, std::string> headerMap_t;
static const std::string Empty = "";


class HTTPRequest
{
	public:
		enum method_t {GET = 0, POST, DELETE, INVALID};

	private:
		std::pair<int, int>	_http_version;
		headerMap_t					_header_map;
		std::string					_raw_request;
		std::string					_uri;
		std::string					_body;
		method_t						_method;

		method_t	extractMethod(const std::string& method);
		const			std::string getNextWord(std::string& line);
		const			std::string& eraseFirstWord(std::string& line);
		const			std::string getNextLine(std::string& line);
		void			extractVersion(std::string httpVersion);
		int				extractHeaderMap(std::string &request);

	public:
		HTTPRequest();
		HTTPRequest(std::string raw_request);
		~HTTPRequest(void);
		HTTPRequest(const HTTPRequest& src);
		HTTPRequest& operator=(const HTTPRequest& rhs);

		const std::string&	getHeaderContent(const std::string& header_title);
		bool								isValid()const;
		bool								isComplete() const;

		std::string	getMethodString(void) const;
		method_t		getMethod(void) const; 
		const				std::string&	getUri(void) const; 
		const 			std::pair<int, int>&	getHTTPVersion(void) const; 
		const 			std::string&	getBody(void) const; 
		const 			std::string&	getRawRequest(void) const; 
		const 			headerMap_t&	getHeaderMap(void) const; 

		void	setBody(const std::string& body);
		int		addHeader(std::string key, std::string value);
		void	setHTTPVersion(const std::string& http_version);
		void 	setHTTPVersion(const int prior, const int minor);
		void 	setHTTPVersion(const std::pair<int, int>& http_version);
		void 	setUri(const std::string& uri);
		void 	setMethod(const HTTPRequest::method_t& method);

};

std::ostream &operator<<(std::ostream &out, const HTTPRequest &stream);

#endif
