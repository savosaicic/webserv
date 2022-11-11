#ifndef CGI_REQUEST_HPP
# define CGI_REQUEST_HPP

<<<<<<< HEAD
#include"HTTPRequest.hpp"
=======
#include "HTTPRequest.hpp"
>>>>>>> 0900768 (Feat: CGIRequest class to execute script)
#include<string>
#include<map>

class CGIRequest
{
<<<<<<< HEAD
	private:
		HTTPRequest		_HTTP_request;
		std::string		_script_path;
		std::string		_input;
		std::string		_path_info;
		std::string		_content_type;

	public:
		CGIRequest();
		~CGIRequest();
		CGIRequest(const HTTPRequest& HTTP_request, const std::string& _script_path);
=======
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
>>>>>>> 0900768 (Feat: CGIRequest class to execute script)
		CGIRequest(const CGIRequest &src);
		CGIRequest& operator=(const CGIRequest &rhs);

		bool isValid();
<<<<<<< HEAD
		void prepareCGI(void);
		void executeCGI(char **envp);
=======
		void executeCGI();
>>>>>>> 0900768 (Feat: CGIRequest class to execute script)
};

#endif
