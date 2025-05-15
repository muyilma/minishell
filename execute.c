#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


int	wait_child(pid_t pid)
{
	int status;
	int exit_code;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		if (exit_code != 0)
			return (exit_code);
	}
}

int ft_execve(t_input *pro, char **args)
{
    char *base;
    char *error_msg;
    
    if (!args || !args[0])
        exit(1);
    
    built_in(args, pro);
    base = check_command_access(args[0], pro->env, &error_msg);
    if (!base)
    {
        ft_print_error("minishell:", error_msg, args, 2);
        if (ft_strncmp(error_msg, ": command not found", 19) == 0 || ft_strncmp(error_msg, ": No such file or directory", 19) == 0)
            exit(127);
        else if (ft_strncmp(error_msg, ": Permission denied", 19) == 0 || ft_strncmp(error_msg, ": Is a directory", 19) == 0)
            exit(126);
        else
            exit(1);
    }
    
    execve(base, args, pro->env);
    free(base);
    ft_print_error("minishell:", ": Failed to execute command", args, 2);
    exit(0);
}

int	execute_last(t_input *pro, int s, int prev_fd)
{
	pid_t pid;
	int res;

	if ((pro->pipe == 0) && pro->arg[s]->str && pro->arg[s]->str[0])
		res = built_in2(pro->arg[s]->str, pro,pro->arg[s]);
	if ((res == 1) || (res == 0))
		return (res);
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
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (wait_child(pid));
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
	}
	close(fd[1]);
	if (*prev_fd != -1)
		close(*prev_fd);
	*prev_fd = fd[0];
}

int	execute_pipe(t_input *pro, int start_idx, int cmd_idx)
{
	int prev_fd = -1;
	int i = start_idx;

	if (!pro->arg[0])
		return 0;
	if (pro->pipe == 0 && pro->arg[start_idx])
		return (execute_last(pro, start_idx, -1));
	while (pro->arg[i] && pro->arg[i + 1])
	{
		execute_command(pro, i, &prev_fd);
		i++;
	}
	if (pro->arg[i])
		return (execute_last(pro, i, prev_fd));
	else if (prev_fd != -1)
		close(prev_fd);
}