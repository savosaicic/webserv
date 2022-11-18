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

bool CGIRequest::isValid()
{
	std::string temp;
	if (_HTTP_request.getMethod() == HTTPRequest::GET)
	{
		
	}
	if (_HTTP_request.getMethod() == HTTPRequest::POST)
	{
		temp = _HTTP_request.getHeaderContent("Content-Length");
		for (std::size_t i = 0;i < temp.size(); i++)
		{
			if (std::isdigit(temp[i]) == 0)
				return (false);
		}
	}
	return (true);
}

void	CGIRequest::prepareCGI(char **path)
{
	std::string temp;

	//Need to parse the location on the host with uri
	_script = "." + _HTTP_request.getUri().substr(0, _HTTP_request.getUri().find_first_of("?"));
	_input = _HTTP_request.getUri().substr(_HTTP_request.getUri().find_first_of("?")+1);
	
  setenv("QUERY_STRING", _input.c_str(), 1);
	path[0] = (char *)_executable.c_str();
	path[1] = (char *)_script.c_str();
	path[2] = NULL;
}


void CGIRequest::executeCGI(char **envp)
{
	int id;
	char *path[3];
	int status;
	int pipe_input[2];
	int pipe_output[2];

	pipe(pipe_input);
	pipe(pipe_output);

	//Not permanent
	write(pipe_input[1], _HTTP_request.getBody().data(), atoi(_HTTP_request.getHeaderContent("Content-Length").c_str()));
	id = fork();
	if (!id)
	{
		prepareCGI(path);

		close(pipe_output[0]);
		close(pipe_input[1]);

		dup2(pipe_output[1], 1);
		dup2(pipe_input[0], 0);

		close(pipe_output[1]);
		close(pipe_input[0]);
		execve(path[0], path, envp);
		perror("Error from child");
	}
	close(pipe_input[1]);
	close(pipe_output[1]);
	close(pipe_input[0]);
	waitpid(id, &status, 0);
	std::string s;
  
   __gnu_cxx::stdio_filebuf<char> pipe_buf(pipe_output[0], std::ios::in);
   std::istream pipe_stream(&pipe_buf);
  
   std::string line;
   while (std::getline(pipe_stream, line))
			 std::cout << line;
  
   close(pipe_output[0]); 
}
