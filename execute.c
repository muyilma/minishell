#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


void	ft_echo(char **args)
{
	int i;
	int newline;

	newline = 1;
	i = 0;
	if (args[0] && ft_strncmp(args[0], "-n", 3) == 0)
	{
		newline = 0;
		i = 1;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}

void	ft_free(char **str)
{
	int i;

	i = 0;
	while (str[i] != 0)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	*pathc(char *cmd)
{
	int i;
	char *tmp = getenv("PATH");
	if (!tmp)
		return (NULL);

	char **path = ft_split(tmp, ':');
	if (!path)
		return (NULL);

	char *arg, *str;
	i = 0;
	while (path[i])
	{
		arg = ft_strjoin(path[i], "/");
		str = ft_strjoin(arg, cmd);
		free(arg);
		if (access(str, X_OK) == 0)
		{
			ft_free(path);
			return (str);
		}
		free(str);
		i++;
	}
	ft_free(path);
	return (NULL);
}

void	ft_execve(char **args)
{
	int i;
	char *base;

	i = 0;
	if (!args || !args[0])
		exit(1);
	if (ft_strncmp(args[0], "echo", 5) == 0)
	{
		ft_echo(&args[1]);
		exit(0);
	}
	base = pathc(args[0]);
	if (!base)
	{
		perror("Command not found");
		exit(1);
	}
	execve(base, args, NULL);
	perror("execve patladi");
	exit(1);
}

void	execute_pipe(t_pro **input)
{
	int fd[2];
	int prev_fd = -1;
	pid_t pid;
	int i = 0;
	int start = 0;
	int s;

	s = 0;
	while (input[s]->str[i] && input[s + 1])
	{
		if (input[s]->str[i + 1] == NULL)
		{
			pipe(fd);
			pid = fork();
			if (pid == 0)
			{
				if (prev_fd != -1)
				{
					dup2(prev_fd, 0);
					close(prev_fd);
				}
				dup2(fd[1], 1);
				close(fd[0]);
				close(fd[1]);	
				ft_execve(&input[s]->str[start]);
			}
			close(fd[1]);
			prev_fd = fd[0];
			s++;
		}
		if (input[s + 1] == NULL)
			break ;
		i++;
	}
	pid = fork();
	if (pid == 0)
	{
		if (prev_fd != -1)
		{
			dup2(prev_fd, 0);
			close(prev_fd);
		}
		ft_execve(&input[s]->str[start]);
	}
	close(prev_fd);
	while (wait(NULL) > 0)
		;
}
