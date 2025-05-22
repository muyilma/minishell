#include "../libft/libft.h"
#include "../minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	built_in3(char **args, t_shell *pro)
{
	if (ft_strncmp(args[0], "unset", 5) == 0)
	{
		ft_unset(&args[1], pro);
		error_and_allocate(pro, 0);
	}
	if (ft_strncmp(args[0], "export", 7) == 0)
	{
		ft_export(&args[1], pro);
		error_and_allocate(pro, 0);
	}
	if (ft_strncmp(args[0], "pwd", 4) == 0)
	{
		ft_pwd();
		error_and_allocate(pro, 0);
	}
	if (ft_strncmp(args[0], "exit", 5) == 0)
	{
		ft_exit(&args[1], pro);
		error_and_allocate(pro, 0);
	}
}

void	built_in(char **args, t_shell *pro)
{
	if (ft_strncmp(args[0], "echo", 5) == 0)
	{
		ft_echo(&args[1]);
		error_and_allocate(pro, 0);
	}
	if (ft_strncmp(args[0], "env", 4) == 0)
	{
		ft_env(&args[1], pro);
		error_and_allocate(pro, 0);
	}
	if (ft_strncmp(args[0], "cd", 3) == 0)
	{
		if (args[1] && args[2])
		{
			ft_print_error(NULL, "minishell: cd: too many arguments", NULL, 1);
			error_and_allocate(pro, 1);
		}
		ft_cd(&args[1], pro);
		error_and_allocate(pro, 0);
	}
	built_in3(args, pro);
}

void	built_in2_redirection2(int *original_stdout, int *original_stdin)
{
	if (*original_stdout != -1)
	{
		dup2(*original_stdout, 1);
		close(*original_stdout);
	}
	if (*original_stdin != -1)
	{
		dup2(*original_stdin, 0);
		close(*original_stdin);
	}
}

void	built_in2_redirection(char **args, t_cmd *arg, int *original_stdout,
		int *original_stdin)
{
	int	fd;

	*original_stdin = -1;
	*original_stdout = -1;
	if (ft_strncmp(args[0], "unset", 5) == 0 || ft_strncmp(args[0], "cd",
			3) == 0 || ft_strncmp(args[0], "unset", 5) == 0
		|| ft_strncmp(args[0], "export", 7) == 0)
	{
		if (arg->heradock)
			redirect_heredoc_write(&fd, arg->heradock, 0);
		if (arg->infile || arg->outfile || arg->append_outfile)
		{
			if (arg->outfile || arg->append_outfile)
				*original_stdout = dup(1);
			if (arg->infile)
				*original_stdin = dup(0);
			handle_redirections(NULL, arg);
		}
	}
}

int	built_in2(char **args, t_shell *pro, t_cmd *arg)
{
	int	result;
	int	original_stdout;
	int	original_stdin;

	built_in2_redirection(args, arg, &original_stdout, &original_stdin);
	if (ft_strncmp(args[0], "exit", 5) == 0)
		result = ft_exit(&args[1], pro);
	else if (ft_strncmp(args[0], "cd", 3) == 0)
	{
		if (args[1] && args[2])
		{
			ft_print_error(NULL, "minishell: cd: too many arguments", NULL, 1);
			result = 1;
		}
		else
			result = ft_cd(&args[1], pro);
	}
	else if (ft_strncmp(args[0], "unset", 5) == 0)
		result = ft_unset(&args[1], pro);
	else if (ft_strncmp(args[0], "export", 7) == 0)
		result = ft_export(&args[1], pro);
	else
		result = 2;
	built_in2_redirection2(&original_stdout, &original_stdin);
	return (result);
}
