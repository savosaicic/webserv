#ifndef CGI_REQUEST_HPP
# define CGI_REQUEST_HPP

#include "HTTPRequest.hpp"
#include<string>
#include<map>

class CGIRequest
{
	public:
		enum executable_t {PHP = 0, PERL, UNKNOWN};
	private:
		HTTPRequest _HTTP_request;
		std::string _script_path;
		executable_t _executable;
		headerMap_t _header_map;
	public:
		CGIRequest();
		~CGIRequest();
		CGIRequest(HTTPRequest HTTP_request, std::string _script_path, executable_t _executable, headerMap_t _header_map);
		CGIRequest(const CGIRequest &src);
		CGIRequest& operator=(const CGIRequest &rhs);

		bool isValid();
		void executeCGI();
};

#endif
