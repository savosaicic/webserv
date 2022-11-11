#ifndef CGI_REQUEST_HPP
# define CGI_REQUEST_HPP

#include"HTTPRequest.hpp"
#include<string>
#include<map>

class CGIRequest
{
	public:
		enum executable_t {PHP = 0, PERL, UNKNOWN};
	private:
		HTTPRequest		_HTTP_request;
		std::string		_script_path;
		executable_t	_executable;
		std::string		_input;
		std::string		_path_info;
		std::string		_content_type;
		std::vector<std::string> _env_variables;

		char **updatedEnvp() const;
		void addCommonHeaders();
	public:
		CGIRequest();
		~CGIRequest();
		CGIRequest(const HTTPRequest& HTTP_request, const std::string& _script_path,
				const executable_t _executable);
		CGIRequest(const CGIRequest &src);
		CGIRequest& operator=(const CGIRequest &rhs);

		bool isValid();
		char **prepareCGI(char **envp);
		void executeCGI(char **envp);
};

#endif
