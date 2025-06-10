/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omgorege <omgorege@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:36:49 by musyilma          #+#    #+#             */
/*   Updated: 2025/05/28 11:11:00 by omgorege         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>

static void	built_in3(char **args, t_shell *pro)
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
	if (ft_strncmp(args[0], "echo", 5) == 0)
	{
		ft_echo(&args[1]);
		error_and_allocate(pro, 0);
	}
}

void	built_in(char **args, t_shell *pro, char *base)
{
	if (ft_strncmp(args[0], "env", 4) == 0)
	{
		base = pathc("ls", pro->env, -1);
		if (base == NULL)
		{
			ft_print_error("minishell:", ": No such file or directory", args,
				2);
			error_and_allocate(pro, 127);
		}
		free(base);
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

static void	built_in2_redirection(char **args, t_cmd *arg, t_shell *pro)
{
	if (ft_strncmp(args[0], "unset", 5) == 0 || ft_strncmp(args[0], "cd",
			3) == 0 || ft_strncmp(args[0], "exit", 5) == 0
		|| ft_strncmp(args[0], "export", 7) == 0)
	{
		if (arg->heradock)
			redirect_heredoc_to_stdin(arg->heradock, 1, pro);
		if (arg->infile || arg->outfile || arg->append_outfile)
			handle_redirections(NULL, arg);
	}
}

int	built_in2(char **args, t_shell *pro, t_cmd *arg)
{
	int	result;

	built_in2_redirection(args, arg, pro);
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
	dup2(pro->original_stdout, 1);
	dup2(pro->original_stdin, 0);
	return (result);
}
