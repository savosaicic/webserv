#include"CGIRequest.hpp"
#include"HTTPRequest.hpp"
#include<string>
#include<map>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sstream>
#include<sys/wait.h>
#include<ext/stdio_filebuf.h>

CGIRequest::CGIRequest()
{
}

CGIRequest::~CGIRequest()
{
}

CGIRequest::CGIRequest(const HTTPRequest& HTTP_request, const std::string& script, const std::string& executable): _HTTP_request(HTTP_request), _script(script), _executable(executable)
{
	_script = "." + _HTTP_request.getUri().substr(0, _HTTP_request.getUri().find_first_of("?"));
	_input = _HTTP_request.getUri().substr(_HTTP_request.getUri().find_first_of("?")+1);

  setenv("CONTENT_LENGTH", HTTP_request.getHeaderContent("Content-Length").c_str(), 1);
  setenv("PATH_INFO", " ", 1);
  setenv("QUERY_STRING", _input.c_str(), 1);
  setenv("REQUEST_METHOD", HTTP_request.getMethodString().c_str(), 1);
  setenv("SCRIPT_NAME", _script.substr(_script.find_last_of("/")).c_str(), 1);
//	setenv("REMOTE_ADDR", HTTP_request.getMethodString().c_str(), 1);
//  setenv("REQUEST_METHOD", , 1);
//  setenv("REQUEST_HOST", , 1);
//  setenv("SERVER_NAME", HTTP_request.getMethodString().c_str(), 1);
  setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
//	setenv("SERVER_SOFTWARE", "webserv/0.1", 1);
	_path[0] = (char *)_executable.c_str();
	_path[1] = (char *)_script.c_str();
	_path[2] = NULL;
}

CGIRequest::CGIRequest(const CGIRequest &src): _HTTP_request(src._HTTP_request), _script(src._script)
{
}


CGIRequest& CGIRequest::operator=(const CGIRequest &rhs)
{
	if (this != &rhs)
	{
		_HTTP_request = rhs._HTTP_request;
		_script = rhs._script;
	}
	return *this;
}

const std::string& CGIRequest::getOutput()
{
	return (_output);
}

bool CGIRequest::executeCGI()
{
	char buffer[BUFFER_SIZE];
	int id;
	int status;
	int pipe_input[2];
	int pipe_output[2];
	int ret;

	pipe(pipe_input);
	pipe(pipe_output);

	ret = write(pipe_input[1], _HTTP_request.getBody().data(),
			atoi(_HTTP_request.getHeaderContent("Content-Length").c_str()));
	if (ret == -1)
		return false;
	id = fork();
	if (!id)
	{
		close(pipe_output[0]);
		close(pipe_input[1]);
		dup2(pipe_output[1], 1);
		dup2(pipe_input[0], 0);
		close(pipe_output[1]);
		close(pipe_input[0]);
		extern char **environ;
		execve(_path[1], &_path[1], environ);
		exit(1);
	}
	close(pipe_input[1]);
	close(pipe_output[1]);
	close(pipe_input[0]);
	while ((ret = read(pipe_output[0], &buffer, BUFFER_SIZE)) > 0)
	  _output.append(buffer, ret);
	if (ret == -1)
		return false;
	waitpid(id, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		return false; 
  close(pipe_output[0]); 
	return true;
}

