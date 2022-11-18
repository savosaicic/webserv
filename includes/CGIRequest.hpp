#ifndef CGI_REQUEST_HPP
# define CGI_REQUEST_HPP

#include "HTTPRequest.hpp"
#include<string>
#include<map>

class CGIRequest
{
	private:
		HTTPRequest		_HTTP_request;
		std::string		_script;
		std::string		_executable;
		std::string		_input;
		std::string		_path_info;
		std::string		_content_type;

	public:
		CGIRequest();
		~CGIRequest();
		CGIRequest(const HTTPRequest& HTTP_request, const std::string& script, const std::string& executable);
		CGIRequest(const CGIRequest &src);
		CGIRequest& operator=(const CGIRequest &rhs);

		bool isValid();
		void prepareCGI(char **path);
		void executeCGI(char **envp);
};

#endif
