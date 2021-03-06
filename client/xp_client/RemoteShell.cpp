
#include <dirent.h>
#include <sys/stat.h>
#include "RemoteShell.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

RemoteShell::RemoteShell(void)
{
	return ;
}

RemoteShell::~RemoteShell(void)
{
	return ;
}

std::vector<std::string>		RemoteShell::stringsplit(std::string s, std::string c)
{
	std::vector<std::string> list;
    size_t pos = 0;
	std::string token;
  
	s = s + c;
	while ((pos = s.find(c)) != std::string::npos) 
	{
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + c.length());
    }
    return list;
}

void		RemoteShell::ChangeDir(std::string command)
{
	std::vector<std::string> 		list;
	std::string						args;
	const char						*strt;

	list = stringsplit(command, " ");
	if (list.size() == 1)
		chdir("/");
	else if (list.size() == 2)
	{
		strt = strchr(command.c_str(), ' ');
		++strt;
		chdir(strt);
	}
}

std::string	RunCommand(std::string to_run, char **args)
{
	extern char **environ;

	std::cout << to_run.c_str() << std::endl;
	execve(to_run.c_str(), args, environ);
}



std::string TraverseDir(std::string dir, std::string comm)
{
#ifndef __APPLE__
	for (auto const &name : dir)
		for (auto const &file : directory_iterator(name))
		{
			std::vector<std::string> filename = stringsplit(file, "/");
			if (filename == comm)
				return (file);
		}
#endif
	struct dirent		*p;
	DIR					*dp;
	struct stat			info;
	char				**args;

	args = cstrsplit(comm.c_str(), ' ');
	if ((dp = opendir(dir.c_str())) == NULL)
		return ("");
	while ((p = readdir(dp)))
	{
		if (strcmp(p->d_name, ".") == 0 || strcmp(p->d_name, "..") == 0)
			continue ;
		std::cout << "p name " << p->d_name << " and comm.cstr() " << comm.c_str() << std::endl;
		if (strcmp(p->d_name, args[0]) == 0)
		{
			std::cout << "here " << std::endl;
			std::string to_run;
			to_run.append(dir);
			to_run.append("/");
			to_run.append(args[0]);
			RunCommand(to_run, args);
			exit(1);
		}
	}
	return ("");
}

void		RemoteShell::ExecveShell(std::string command)
{
	std::vector<std::string> 		list;
	std::string						args;
	int								i;
	std::string						pathway;
	std::vector<std::string>		path;

	extern char						**environ;

	list = stringsplit(command, " ");
	i = -1;
	while (environ[++i])
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			std::cout << "PATH found " << std::endl;
			pathway = environ[i];
			break ;
		}
	path = stringsplit(pathway, "=");
	std::string directories = path.at(1);
	std::vector<std::string> listing = stringsplit(directories, ":");
	for (auto const &plc : listing)
		std::string exec_string = TraverseDir(plc, command);
}

int	 main(void)
{
	RemoteShell		shell;

	shell.ChangeDir("cd ..");
	shell.ExecveShell("ls");
	return (0);
}

