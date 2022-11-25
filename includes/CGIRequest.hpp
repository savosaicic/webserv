#ifndef CGI_REQUEST_HPP
# define CGI_REQUEST_HPP

#include "HTTPRequest.hpp"
#include "HTTPStream.hpp"
#include <string>
#include <map>

#define BUFFER_SIZE 4096

class CGIRequest
{
	private:
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
		CGIRequest(const HTTPStream& stream, const std::string& rooted_script,
        const std::string& executable);
		CGIRequest(const CGIRequest &src);
		CGIRequest& operator=(const CGIRequest &rhs);

    bool executeCGI(const HTTPRequest& request);
		const std::string& getOutput();
};

#endif
