#include "libft/libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

int	quotes_skip(char *str, int j)
{
	int		i;
	char	qut;

	i = j;
	if (str[i] == 34 || str[i] == 39)
	{
		qut = str[i++];
		while (str[i] && str[i] != qut)
			i++;
		if (str[i])
			i++;
	}
	return (i);
}

void	empty_line(t_input *a)
{
	int	i;

	i = 0;
	while (a->input[i])
	{
		if (a->input[i] == 34)
		{
			i++;
			a->isprint++;
			while (a->input[i] != 34)
			{
				if (a->input[i] == '$')
					a->dollar++;
				i++;
			}
		}
		if (a->input[i] == 39)
		{
			i = quotes_skip(a->input, i);
			a->isprint++;
		}
		if (a->input[i] != ' ')
			a->isprint++;
		if (a->input[i] == '$')
			a->dollar++;
		if (a->input[i] == '|' || a->input[i] == '<' || a->input[i] == '>')
			a->operator++;
		if (a->input[i] == '|')
			a->pipe++;
		i++;
	}
	if (a->isprint == 0)
		a->error = 1;
}

int	op_checker(t_input *input, int i)
{
	int	j;

	j = i;
	input->after_str = 0;
	if (input->input[i] == '|' && input->isalpha == 0)
		return (input->error = 3, i);
	if (input->input[i] == '|')
		input->isalpha = 0;
	if (input->input[i] == '<' || input->input[i] == '>')
	{
		if (input->input[i + 1] == '<' || input->input[i + 1] == '>')
		{
			i++;
			j++;
		}
	}
	while (input->input[++i] && input->input[i] != '|')
	{
		if (input->input[i] != ' ')
			input->after_str++;
	}
	if (input->after_str == 0)
		input->error = 3;
	return (j); // burdaki +1'i sildin
}
void	opCounter(t_input *input)
{
	int	i;

	i = 0;
	if (input->error > 0)
		return ;
	while (input->input[i])
	{
		if (input->input[i] == 34 || input->input[i] == 39)
		{
			input->qut = input->input[i];
			i++;
			while (input->input[i] != input->qut)
				i++;
			input->isalpha++;
		}
		else if (input->input[i] == '|' || input->input[i] == '<'
			|| input->input[i] == '>')
		{
			// printf("1-%s\n",input->input+i);
			i = op_checker(input, i);
		}
		else if (input->input[i] != ' ')
			input->isalpha++;
		if (input->error > 0)
			return ;
		i++;
	}
}


void	ft_parser(t_input *input)
{
	int	i;

	i = -1;
	empty_line(input);
	while (input->input[++i])
	{
		if (input->input[i] == 34 || input->input[i] == 39)
		{
			input->quotes++;
			input->qut = input->input[i];
			i++;
			while (input->input[i] != input->qut && input->input[i] != '\0')
				i++;
			if (input->input[i] == input->qut)
				input->quotes++;
		}
	}
	if (input->quotes % 2 != 0)
		input->error = 2;
	if (input->dollar > 0)
		dollar_parse(input);
	if (input->operator> 0)
		opCounter(input);
	arg_parse(input, ft_strlen(input->input), 0);
}
