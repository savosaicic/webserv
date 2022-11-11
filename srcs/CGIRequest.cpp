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

CGIRequest::CGIRequest()
{

}

CGIRequest::~CGIRequest()
{

}

CGIRequest::CGIRequest(const HTTPRequest& HTTP_request, const std::string& script_path, const executable_t executable): _HTTP_request(HTTP_request), _script_path(script_path), _executable(executable)
{


}

CGIRequest::CGIRequest(const CGIRequest &src)
{
	_HTTP_request = src._HTTP_request;
	_script_path = src._script_path;
	_executable = src._executable;
}

CGIRequest& CGIRequest::operator=(const CGIRequest &rhs)
{
	_HTTP_request = rhs._HTTP_request;
	_script_path = rhs._script_path;
	_executable = rhs._executable;
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

void CGIRequest::addCommonHeaders()
{
	std::stringstream ss;
	std::string temp;

	_input = "GATEWAY_INTERFACE=CGI/1.0";
	_env_variables.push_back(_input);		
	std::cout << " hello :"  << _HTTP_request.getHTTPVersion().first << '.' << _HTTP_request.getHTTPVersion().second << std::endl;
	ss << _HTTP_request.getHTTPVersion().first << "." << _HTTP_request.getHTTPVersion().second;
	ss >> temp;
	_input = "SERVER_PROTOCOL=HTTP/" + temp;
	_env_variables.push_back(_input);		
	_input = "REQUEST_METHOD=" + _HTTP_request.getMethodString();
	_env_variables.push_back(_input);		
}

char **CGIRequest::updatedEnvp() const
{
	char **new_envp;

	new_envp = new char*[_env_variables.size()+1];
	for (std::size_t i = 0;i < _env_variables.size();i++)
	{
		new_envp[i] = new char[_env_variables[i].length()+1];
		for (std::size_t j = 0;j < _env_variables[i].length();j++)
			new_envp[i][j] = _env_variables[i][j];
		new_envp[i][_env_variables[i].length()] = '\0';
	}	
	new_envp[_env_variables.size()] = NULL;
	return (new_envp);
}

char** CGIRequest::prepareCGI(char **envp)
{
	std::string temp;
	char **new_envp;

	for(int i = 0;envp[i];i++)
		_env_variables.push_back(std::string(envp[i]));
	if (_HTTP_request.getMethod() == HTTPRequest::GET)
	{
		//Need to parse the location on the host with uri
		_script_path = "." + _HTTP_request.getUri().substr(0, _HTTP_request.getUri().find_first_of("?"));
		_input = _HTTP_request.getUri().substr(_HTTP_request.getUri().find_first_of("?")+1);
		_input = "QUERY_STRING="+_input;
		_env_variables.push_back(_input);		
	}
	else if (_HTTP_request.getMethod() == HTTPRequest::POST)
	{
		
	}
	addCommonHeaders();
	new_envp = updatedEnvp();
	return (new_envp);
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


		new_envp = prepareCGI(envp);

		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		close(pipe_fd[1]);
		execve(path[0], path, new_envp);

		perror("Error from child");
	}
	waitpid(id, &status, 0);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);

	std::string s;
	while (std::getline(std::cin,s))
		std::cout << s << '\n';
}
