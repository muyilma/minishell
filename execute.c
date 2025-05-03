#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	ft_execve(t_input *pro, char **args)
{
	int i;
	char *base;

	i = 0;
	if (!args || !args[0])
		exit(1);
	built_in(args, pro->environ);
	base = pathc(args[0], pro->environ);
	if (!base)
	{
		printf("Command not found:%s\n", args[0]);
		exit(1);
	}
	execve(base, args, NULL);
	perror("execve patladi");
	exit(1);
}

void	execute_last(t_input *pro, int s, int prev_fd)
{
	pid_t pid;

	pid = fork();
	if (pid == 0)
	{
		if (prev_fd != -1)
		{
			dup2(prev_fd, 0);
			close(prev_fd);
		}
		handle_redirections(pro->arg[s]);
		ft_execve(pro, &pro->arg[s]->str[0]);
	}
	close(prev_fd);
	while (wait(NULL) > 0)
		;
}
int 	execute_pipev2(t_input *pro, int s, int i, int prev_fd)
{
	int fd[2];
	pid_t pid;

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
		handle_redirections(pro->arg[s]);
		ft_execve(pro, &pro->arg[s]->str[0]);
	}
	close(fd[1]);
	prev_fd = fd[0];
}

void	execute_pipe(t_input *pro, int s, int i)
{
	int prev_fd;
	
	prev_fd = -1;
	while (pro->arg[s]->str && pro->arg[s]->str[i] && pro->arg[s + 1])
	{
		if (pro->arg[s]->str[i + 1] == NULL)
		{
			execute_pipev2(pro, s, i, prev_fd);
			s++;
		}
		if (pro->arg[s + 1] == NULL)
			break ;
		i++;
	}
	execute_last(pro, s, prev_fd);
}
