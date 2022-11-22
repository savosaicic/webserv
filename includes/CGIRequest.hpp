#ifndef CGI_REQUEST_HPP
# define CGI_REQUEST_HPP

#include "HTTPRequest.hpp"
#include<string>
#include<map>

#define BUFFER_SIZE 4096

class CGIRequest
{
	private:
		HTTPRequest	_HTTP_request;
		std::string	_script;
		std::string	_executable;
		std::string	_input;
		std::string	_path_info;
		std::string	_content_type;
		char*				_path[3];
		std::string	_output;

	public:
		CGIRequest();
		~CGIRequest();
		CGIRequest(const HTTPRequest& HTTP_request, const std::string& script, const std::string& executable);
		CGIRequest(const CGIRequest &src);
		CGIRequest& operator=(const CGIRequest &rhs);

		bool executeCGI(char **envp);
		const std::string& getOutput();
};

#endif
