#include "minishell.h"
#include "libft/libft.h"

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>


void	read_line(t_input *input,char **env,int code)
{
	input->input = NULL;
	input->isprint = 0;
	input->isalpha = 0;
	// input->backslash = 0;
	input->pipe = 0;
	input->error = 0;
	input->operator= 0;
	// input->dot = 0;
	input->after_str = 0;
	input->quotes = 0;
	input->dollar = 0;
	input->env = env;
	input->arg=NULL;
	input->exit_code=code;

	input->input = readline("minishell:");
	if (!input->input)
	{
		perror("hata");
		exit(1);
	}
	add_history(input->input);
}

void	ft_executer_free(t_input *input)
{
	int i;
	int j;

	i = 0;
	j = 0;
	if (input->arg)
	{
		while (input->arg[i])
		{
			if (input->arg[i]->str)
			{
				while (input->arg[i]->str[j])
				{
					free(input->arg[i]->str[j]);
					j++;
				}
				free(input->arg[i]->str);
				j = 0;
			}

			if (input->arg[i]->append_outfile)
				free(input->arg[i]->append_outfile);
			if (input->arg[i]->heradock)
				free(input->arg[i]->heradock);
			if (input->arg[i]->infile)
			{
				printf("-----\n");
				free(input->arg[i]->infile);
			}
			if (input->arg[i]->outfile)
				free(input->arg[i]->outfile);
			free(input->arg[i]);
			i++;
		}
	}

	free(input->arg);
}

int	ft_executer(t_input *input)
{
	int exit;
	free(input->input);
	
	exit = execute_pipe(input, 0, 0);
	//ft_executer_free(input);

	free(input);
	return (exit);
}

void	ft_error(t_input *input)
{
	if (input->error == 2)
		perror("minishell: open quotes \"\'");
	else if (input->error == 3)
		perror("minishell: syntax error near unexpected token `newline'\n");
	free(input->input);
	free(input);
}

int	main(int ac, char **av, char **env)
{
	t_input *input;
	int exit_code;

	exit_code=0;
	while (1)
	{
		input = malloc(sizeof(t_input));
		read_line(input,env,exit_code);
		ft_parser(input);
		if (input->error == 0)
		{
			exit_code = ft_executer(input);
		}
		else
			ft_error(input);
	}
}
