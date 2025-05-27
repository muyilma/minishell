/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:50:38 by musyilma          #+#    #+#             */
/*   Updated: 2025/05/27 16:55:41 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int		g_signal_exit = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	if (g_signal_exit == 0 || g_signal_exit == 130)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_signal_exit == 2)
	{
		write(1, "\n", 1);
		rl_on_new_line();
	}
	else if (g_signal_exit == 1)
	{
		set_exit_status_code(130);
		exit(*get_exit_status_code());
	}
	set_exit_status_code(130);
	g_signal_exit = 130;
}

void	read_line(t_shell *input, char **env)
{
	input->original_stdin = dup(0);
	input->original_stdout = dup(1);
	input->input = NULL;
	input->isprint = 0;
	input->isalpha = 0;
	input->pipe = 0;
	input->error = 0;
	input->operator = 0;
	input->after_str = 0;
	input->quotes = 0;
	input->dollar = 0;
	input->env = env;
	input->arg = NULL;
	input->temp = NULL;
	input->input = readline("minishell:");
	if (!input->input)
	{
		ft_print_error(NULL, "exit", NULL, 1);
		error_and_allocate(input, 0);
	}
	add_history(input->input);
}

int	ft_executer(t_shell *input, char ***new_env)
{
	int	exit;

	free(input->input);
	exit = execute_pipe(input, 0);
	ft_executer_free(input);
	g_signal_exit = 0;
	if (input->env != *new_env)
		*new_env = input->env;
	free(input);
	return (exit);
}

int	ft_error(t_shell *input)
{
	if (input->error == 2)
		write(2, "minishell: open quotes \"\'\n", 27);
	else if (input->error == 3)
		write(2, "minishell: syntax error near unexpected token `newline'\n",
			57);
	if (input->error == 2 || input->error == 3)
	{
		free(input->input);
		free(input);
		return (2);
	}
	free(input->input);
	free(input);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_shell	*input;
	char	**new_env;

	(void)av;
	(void)ac;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	new_env = copy_env(env, 0);
	while (1)
	{
		g_signal_exit = 0;
		input = malloc(sizeof(t_shell));
		if (!input)
			return (0);
		read_line(input, new_env);
		ft_parser(input);
		if (input->error == 0)
			set_exit_status_code(ft_executer(input, &new_env));
		else
			set_exit_status_code(ft_error(input));
		if (g_signal_exit == 130)
			set_exit_status_code(130);
	}
}
