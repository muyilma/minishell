#include "../libft/libft.h"
#include "../minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int wait_child(pid_t pid)
{
	int status;
	int exit_code = 0;
	pid_t ended_pid;

	while ((ended_pid = wait(&status)) > 0)
	{
		if (ended_pid == pid)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
		}
	}
	return (exit_code);
}

int	ft_execve(t_shell *pro, char **args)
{
	char	*base;
	char	*error_msg;

	if (!args || !args[0])
	{
		error_and_allocate(pro, 1);
	}
	built_in(args, pro);
	base = check_command_access(args[0], pro->env, &error_msg);
	if (!base)
	{
		ft_print_error("minishell:", error_msg, args, 2);
		if (ft_strncmp(error_msg, ": command not found", 19) == 0
			|| ft_strncmp(error_msg, ": No such file or directory", 19) == 0)
			error_and_allocate(pro, 127);
		else if (ft_strncmp(error_msg, ": Permission denied", 19) == 0
			|| ft_strncmp(error_msg, ": Is a directory", 19) == 0)
			error_and_allocate(pro, 126);
		else
			error_and_allocate(pro, 1);
	}
	execve(base, args, pro->env);
	free(base);
	ft_print_error("minishell:", ": Failed to execute command", args, 2);
	error_and_allocate(pro, 1);
}

int	execute_last(t_shell *pro, int s, int prev_fd)
{
	pid_t	pid;
	int		res;

	res = 2;
	if ((pro->pipe == 0) && pro->arg[s]->str && pro->arg[s]->str[0])
		res = built_in2(pro->arg[s]->str, pro, pro->arg[s]);
	if (res != 2)
		return (res);
	res = heredoc_control(pro->arg[s]);
	pid = fork();
	if (pid == 0)
	{
		if (res == 1 && prev_fd != -1)
		{
			dup2(prev_fd, 0);
			close(prev_fd);
		}
		handle_redirections(pro, pro->arg[s]);
		ft_execve(pro, pro->arg[s]->str);
	}
	if (prev_fd != -1)
		close(prev_fd);
	dup2(pro->original_stdin, 0);
	return (wait_child(pid));
}

void	execute_command(t_shell *pro, int cmd_index, int *prev_fd)
{
	int		fd[2];
	pid_t	pid;
	int		heredoc;

	heredoc = heredoc_control(pro->arg[cmd_index]);
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		if (heredoc == 1 && *prev_fd != -1)
		{
			dup2(*prev_fd, 0);
			close(*prev_fd);
		}
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		handle_redirections(pro, pro->arg[cmd_index]);
		ft_execve(pro, pro->arg[cmd_index]->str);
	}
	close(fd[1]);
	if (*prev_fd != -1)
		close(*prev_fd);
	*prev_fd = fd[0];
}

int	execute_pipe(t_shell *pro, int start_idx)
{
	int	prev_fd;
	int	i;

	prev_fd = -1;
	i = start_idx;
	if (!pro->arg[0])
		return (0);
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
	return (0);
}
