#include "libft/libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_sigint(int sig)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void read_line(t_input *input, char **env, int code)
{
	input->input = NULL;
	input->isprint = 0;
	input->isalpha = 0;
	input->pipe = 0;
	input->error = 0;
	input->operator = 0;
	// input->dot = 0;
	input->after_str = 0;
	input->quotes = 0;
	input->dollar = 0;
	input->env = env;
	input->arg = NULL;
	input->exit_code = code;
	input->input = readline("minishell:");
	if (!input->input)
	{
		//ft_print_error(NULL, "exit", NULL, 1);
		exit(0);
	}
	add_history(input->input);
}

int ft_executer(t_input *input)
{
	// 	int	l;
	// int k;
	// k = 0;
	// l = 0;
	// while (input->arg[k])
	// {
	// 	if (input->arg[k]->str)
	// 	{

	// 		while (input->arg[k]->str[l])
	// 		{
	// 			printf("ipt->arg[%d]->str[%d] %s\n", k, l,
	// 				input->arg[k]->str[l]);
	// 			l++;
	// 		}
	// 		l=0;
	// 	}
	// 	if (input->arg[k]->infile)
	// 		printf("ipt->arg[%d]->infile %s\n",k,input->arg[k]->infile);
	// 	if (input->arg[k]->outfile)
	// 		printf("ipt->arg[%d]->outfile %s\n",k,input->arg[k]->outfile);

	// 	k++;
	// }
	// printf("---\n");
	// if (input->arg[0])
	// {
	// 	printf("oluştu\n");
	// }
	// else
	// {
		
	// 	printf("oluşmadı\n");
	// }
	// printf("---\n");
	int exit;
	free(input->input);
	exit = execute_pipe(input, 0, 0);

	ft_executer_free(input);
	free(input);
	return (exit);
}

void ft_error(t_input *input)
{
	if (input->error == 2)
		write(2, "minishell: open quotes \"\'", 26);
	else if (input->error == 3)
		write(2, "minishell: syntax error near unexpected token `newline'\n", 57);
	free(input->input);
	free(input);
}

int main(int ac, char **av, char **env)
{
	t_input *input;
	int exit_code;

	signal(SIGINT, handle_sigint);
	exit_code = 0;
	while (1)
	{
		input = malloc(sizeof(t_input));
		read_line(input, env, exit_code);
		ft_parser(input);
		if (input->error == 0)
		{
			exit_code = ft_executer(input);
		}
		else
			ft_error(input);
	}
}