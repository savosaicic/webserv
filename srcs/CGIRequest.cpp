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
#include<sys/wait.h>

CGIRequest::CGIRequest()
{

}

CGIRequest::~CGIRequest()
{
}

CGIRequest::CGIRequest(const HTTPRequest& HTTP_request, const std::string& script_path): _HTTP_request(HTTP_request), _script_path(script_path)
{
}

CGIRequest::CGIRequest(const CGIRequest &src): _HTTP_request(src._HTTP_request), _script_path(src._script_path)
{
}


CGIRequest& CGIRequest::operator=(const CGIRequest &rhs)
{
	if (this != this)
	{
		_HTTP_request = rhs._HTTP_request;
		_script_path = rhs._script_path;
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

void	CGIRequest::prepareCGI(void)
{
	std::string temp;

	//Need to parse the location on the host with uri
	_script_path = "." + _HTTP_request.getUri().substr(0, _HTTP_request.getUri().find_first_of("?"));
	_input = _HTTP_request.getUri().substr(_HTTP_request.getUri().find_first_of("?")+1);
	
  setenv("QUERY_STRING", _input.c_str(), 1);

}

void CGIRequest::executeCGI(char **envp)
{
	int id;
	char **new_envp;
	char **path;
	int status;
	int pipe_fd[2];

	pipe(pipe_fd);

	id = fork();
	if (!id)
	{
		path = new char*[3];
		path[0] = (char *)std::string("/usr/bin/perl").c_str();
		path[1] = (char *)_script_path.c_str();
		path[2] = NULL;

		prepareCGI();

		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		close(pipe_fd[1]);
		execve(path[0], path, new_envp);

    /*
     * char argv[2];
     * argv[0] = _script_path.c_str();
     * argv[1] = NULL;
     *
     * setenv("REQUEST_METHOD", _http.method(), 1);
     * setenv("WHATEVER", _http.whatever(), 1);
     *
     * dup2(fds[1], STDOUT);
     * close(fds[0]);
     * close(fds[1]);
     *
     * execve(_script_path, argv, prepareCGI());
     *
    */

		perror("Error from child");
	}
	waitpid(id, &status, 0);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);

	std::string s;
	while (std::getline(std::cin,s))
		std::cout << s << '\n';

  /*
   * close(fds[1]);
   * waitpid(id, &status, 0);
   *
   * __gnu_cxx::stdio_filebuf<char> pipe_buf(fds[0], std::ios::in);
   * std::istream pipe_stream(&pipe_buf);
   *
   * std::string line;
   * while (std::getline(pipe_stream, line);
   *   cout << line;
   *
   * close(fds[0]);
  */
}
