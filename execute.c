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
	if (built_in(args, pro) == 1)
		return ;
	base = pathc(args[0], pro->env);
	if (!base)
	{
		printf("Command not found: %s\n", args[0]);
		exit(1);
	}
	execve(base, args, pro->env);
	perror("execve failed");
	free(base);
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
		ft_execve(pro, pro->arg[s]->str);
		exit(1);
	}

	if (prev_fd != -1)
		close(prev_fd);
	while (wait(NULL) > 0)
		;
}

void	execute_command(t_input *pro, int cmd_index, int *prev_fd)
{
	int fd[2];
	pid_t pid;

    pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		if (*prev_fd != -1)
		{
			dup2(*prev_fd, 0);
			close(*prev_fd);
		}
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		handle_redirections(pro->arg[cmd_index]);
		ft_execve(pro, pro->arg[cmd_index]->str);
		exit(1);
	}
	close(fd[1]);
	if (*prev_fd != -1)
		close(*prev_fd);
	*prev_fd = fd[0];
}

void	execute_pipe(t_input *pro, int start_idx, int cmd_idx)
{
	int prev_fd = -1;
	int i = start_idx;

	if (pro->pipe == 0 && pro->arg[start_idx])
	{
		execute_last(pro, start_idx, -1);
		return ;
	}
	while (pro->arg[i] && pro->arg[i + 1])
	{
		execute_command(pro, i, &prev_fd);
		i++;
	}
	if (pro->arg[i])
		execute_last(pro, i, prev_fd);
	else if (prev_fd != -1)
		close(prev_fd);
}
