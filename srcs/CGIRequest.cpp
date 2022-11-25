#include"CGIRequest.hpp"
#include"HTTPRequest.hpp"
#include <iostream>
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
	
  setenv("QUERY_STRING", _input.c_str(), 1);
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

bool CGIRequest::executeCGI(char **envp)
{
	char buffer[BUFFER_SIZE];
	int id;
	int status;
	int pipe_input[2];
	int pipe_output[2];
	int ret;

	pipe(pipe_input);
	pipe(pipe_output);

	write(pipe_input[1], _HTTP_request.getBody().data(),
			atoi(_HTTP_request.getHeaderContent("Content-Length").c_str()));
	id = fork();
	if (!id)
	{
		close(pipe_output[0]);
		close(pipe_input[1]);
		dup2(pipe_output[1], 1);
		dup2(pipe_input[0], 0);
		close(pipe_output[1]);
		close(pipe_input[0]);
		execve(_path[0], _path, envp);
		exit(1);
	}
	close(pipe_input[1]);
	close(pipe_output[1]);
	close(pipe_input[0]);
	waitpid(id, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		return (false); 
	while ((ret = read(pipe_output[0], &buffer, BUFFER_SIZE)) > 0)
	  _output.append(buffer, ret);
  std::cerr << "...fddfs\n";
  close(pipe_output[0]); 
	return (true);
}

