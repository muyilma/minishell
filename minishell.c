#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

void	read_line(t_input *input)
{
	input->input = NULL;
	input->isprint = 0;
	input->isalpha = 0;
	// input->backslash = 0;
	// input->pipe = 0;
	input->error = 0;
	input->operator= 0;
	// input->dot = 0;
	input->after_str = 0;
	input->quotes = 0;
	input->dollar = 0;
	input->input = readline("yaz:");
	add_history(input->input);
}

void	ft_execve(t_input *input)
{
	printf("%s\n", input->input);
	free(input->input);
	free(input);
}

void	ft_error(t_input *input)
{
	if (input->error==2)
		printf("minishell: open quotes \"\'");
	else if (input->error == 3)
		printf("minishell: syntax error near unexpected token `newline'\n");
	free(input->input);
	free(input);
}

int	main(void)
{
	t_input	*input;

	while (1)
	{
		input = malloc(sizeof(t_input));
		read_line(input);
		ft_parser(input);
		if (input->error == 0)
			ft_execve(input);
		else
			ft_error(input);
	}
}
