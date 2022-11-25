#include "CGIRequest.hpp"
#include "HTTPRequest.hpp"
#include "HTTPStream.hpp"
#include "http_utils.hpp"
#include <fcntl.h>
#include <iostream>
#include <string>
#include <map>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <sys/wait.h>
#include <ext/stdio_filebuf.h>

CGIRequest::CGIRequest()
{
}

CGIRequest::~CGIRequest()
{
}

CGIRequest::CGIRequest(const HTTPStream& stream,
    const std::string& rooted_script,
    const std::string& executable)
  : _script(rooted_script), _executable(executable)
{
	_script = "." +
    stream.getRequest().getUri().substr(0,
        stream.getRequest().getUri().find_first_of("?"));
	_input = stream.getRequest().getUri().substr(
      stream.getRequest().getUri().find_first_of("?")+1);

  setenv("CONTENT_LENGTH",
      stream.getRequest().getHeaderContent(http_headers::content_length).c_str(), 1);
  setenv("PATH_INFO", "", 1);
  setenv("QUERY_STRING", "", 1);
  setenv("REQUEST_METHOD", stream.getRequest().getMethodString().c_str(), 1);
  setenv("SCRIPT_NAME", _script.substr(_script.find_last_of("/")).c_str(), 1);
	setenv("REMOTE_ADDR", stream.tcp().getIPAddr().toStr().c_str() , 1);
//  setenv("REQUEST_HOST", , 1);
  setenv("SERVER_NAME",
      stream.getRequest().getHeaderContent(http_headers::host).c_str(), 1);
  setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("SERVER_SOFTWARE", "webserv/0.1", 1);
	setenv("DOCUMENT_ROOT", "/mnt/nfs/homes/sasaicic/webserv/www", 1);
	setenv("CONTENT_TYPE",
      stream.getRequest().getHeaderContent(http_headers::content_type).c_str(), 1);
	_path[0] = (char *)_executable.c_str();
	_path[1] = (char *)rooted_script.c_str();
	_path[2] = NULL;
}

CGIRequest::CGIRequest(const CGIRequest &src) : _script(src._script), 
  _executable(src._executable), _input(src._input), _path_info(src._path_info),
  _content_type(src._content_type), _output(src._output) 
{
  _path[0] = src._path[0];
  _path[1] = src._path[1];
  _path[2] = src._path[2];
}


CGIRequest& CGIRequest::operator=(const CGIRequest &rhs)
{
	if (this != &rhs)
	{
		_script = rhs._script;
		_executable = rhs._script;
		_input = rhs._input;
		_path_info = rhs._path_info;
		_content_type = rhs._content_type;
		_path[0] = rhs._path[0];
		_path[1] = rhs._path[1];
		_path[2] = rhs._path[2];
		_output = rhs._output;
	}
	return *this;
}

const std::string& CGIRequest::getOutput()
{
	return (_output);
}

bool CGIRequest::executeCGI(const HTTPRequest& request)
{
	char buffer[BUFFER_SIZE];
	int id;
	int status;
	int pipe_input[2];
	int pipe_output[2];
	int ret;

	pipe(pipe_input);
	pipe(pipe_output);

	id = fork();
	if (!id)
	{
		close(pipe_output[0]);
		close(pipe_input[1]);
		dup2(pipe_output[1], STDOUT_FILENO);
		dup2(pipe_input[0], STDIN_FILENO);
		close(pipe_output[1]);
		close(pipe_input[0]);
		extern char **environ;
		execve(_path[0], _path, environ);
		exit(1);
	}

  int flags = fcntl(pipe_output[1], F_GETFL, 0);
  fcntl(pipe_output[1], F_SETFL, flags | O_NONBLOCK);

  std::string tmp(request.getBody());
	while (1) {
    ret = write(pipe_input[1], tmp.data(), tmp.size());
    if (ret <= 0 || ret == (int)tmp.size())
      break ;
    tmp.erase(0, ret);
  }
  if (ret == -1)
    return false;

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

