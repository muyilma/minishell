/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:38:10 by musyilma          #+#    #+#             */
/*   Updated: 2025/05/27 18:03:42 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>

int	wait_child(pid_t pid)
{
	int		status;
	pid_t	ended_pid;

	set_exit_status_code(0);
	ended_pid = wait(&status);
	while (ended_pid > 0)
	{
		if (ended_pid == pid && WIFEXITED(status))
			set_exit_status_code(WEXITSTATUS(status));
		ended_pid = wait(&status);
	}
	return (*get_exit_status_code());
}

static void	ft_execve(t_shell *pro, char **args)
{
	char	*base;
	char	*error_msg;

	if (!args || !args[0])
		error_and_allocate(pro, *get_exit_status_code());
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
	ft_print_error("minishell:", ": command not found", args, 2);
	error_and_allocate(pro, 1);
}

static int	execute_last(t_shell *pro, int s, int prev_fd, int res)
{
	pid_t	pid;

	if ((pro->pipe == 0) && pro->arg[s]->str && pro->arg[s]->str[0])
		res = built_in2(pro->arg[s]->str, pro, pro->arg[s]);
	if (res != 2)
		return (res);
	res = heredoc_control(pro->arg[s], pro->original_stdin, pro);
	if (res == 130)
		return (res);
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

static void	execute_command(t_shell *pro, int cmd_index, int *prev_fd,
		int *heredoc)
{
	int		fd[2];
	pid_t	pid;

	*heredoc = heredoc_control(pro->arg[cmd_index], pro->original_stdin, pro);
	if (*heredoc == 130)
		return ;
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		if (*heredoc == 1 && *prev_fd != -1)
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
	int	heredoc;

	g_signal_exit = 2;
	prev_fd = -1;
	i = start_idx;
	if (!pro->arg[0])
		return (0);
	if (pro->pipe == 0 && pro->arg[start_idx])
		return (execute_last(pro, start_idx, -1, 2));
	while (pro->arg[i] && pro->arg[i + 1])
	{
		execute_command(pro, i, &prev_fd, &heredoc);
		if (heredoc == 130)
			return (130);
		i++;
	}
	if (pro->arg[i])
		return (execute_last(pro, i, prev_fd, 2));
	else if (prev_fd != -1)
		close(prev_fd);
	return (0);
}
