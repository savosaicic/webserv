#include "CGIRequest.hpp"
#include "HTTPRequest.hpp"
#include<string>
#include<map>

CGIRequest::CGIRequest()
{

}

CGIRequest::~CGIRequest()
{

}

CGIRequest::CGIRequest(HTTPRequest HTTP_request, std::string script_path, executable_t executable, headerMap_t header_map): _HTTP_request(HTTP_request), _script_path(script_path), _executable(executable), _header_map(header_map)
{


}

CGIRequest::CGIRequest(const CGIRequest &src)
{
	_HTTP_request = src._HTTP_request;
	_script_path = src._script_path;
	_executable = src._executable;
	_header_map = src._header_map;
}

CGIRequest& CGIRequest::operator=(const CGIRequest &rhs)
{
	_HTTP_request = rhs._HTTP_request;
	_script_path = rhs._script_path;
	_executable = rhs._executable;
	_header_map = rhs._header_map;
	return *this;
}
