#include "../libft/libft.h"
#include "../minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>


int	quotes_operator_counter(t_shell *a, int i)
{
	if (a->input[i] == 34)
	{
		i++;
		a->isprint++;
		while (a->input[i] != 34 && a->input[i] != '\0')
		{
			if (a->input[i] == '$')
				a->dollar++;
			i++;
		}
		if (a->input[i] == 34)
		{
			i++;
		}
	}
	if (a->input[i] == 39)
	{
		i = quotes_skip(a->input, i, 0,0);
		a->isprint++;
	}
	return (i - 1);
}

void	check_empty_line(t_shell *a)
{
	int i;

	i = 0;
	while (a->input[i])
	{
		if (a->input[i] == 34 || a->input[i] == 39)
			i = quotes_operator_counter(a, i);
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

int	op_checker2(t_shell *input, int i, int j,int flag)
{
	while (input->input[++i] && input->input[i] != '|')
	{
		if (input->input[i] == 34 || input->input[i] == 39)
		{
			i = quotes_skip(input->input, i, 0,0);
			input->after_str++;
		}
		if (input->input[i] == '<' || input->input[i] == '>')
		{
			if (input->after_str == 0 && flag != 0)
			{
				input->error = 3;
				break ;
			}
			flag++;
		}
		if (input->input[i] != ' ')
			input->after_str++;
	}
	if (input->after_str == 0)
		input->error = 3;
	return (j); // burdaki +1'i sildin
}

int	op_checker(t_shell *input, int i)
{
	int j;
	int flag;

	flag=1;
	j = i;
	input->after_str = 0;
	if (input->input[i] == '|' && input->isalpha == 0)
		return (input->error = 3, i);
	if (input->input[i] == '|')
	{
		input->isalpha = 0;
		flag=0;
	}
	if (input->input[i] == '<' || input->input[i] == '>')
	{
		if (input->input[i + 1] == '<' || input->input[i + 1] == '>')
		{
			i++;
			j++;
		}
	}
	return (op_checker2(input, i, j,flag));
}

void	operator_control(t_shell *a)
{
	int i;

	i = 0;
	while (a->input[i])
	{
		if (a->input[i] == 34 || a->input[i] == 39)
		{
			a->qut = a->input[i];
			i++;
			while (a->input[i] != a->qut)
				i++;
			a->isalpha++;
		}
		else if (a->input[i] == '|' || a->input[i] == '<'
			|| a->input[i] == '>')
			i = op_checker(a, i);
		else if (a->input[i] != ' ')
			a->isalpha++;
		if (a->error > 0)
			return ;
		i++;
	}
}

void	ft_parser(t_shell *input)
{
	check_empty_line(input);
	if (input->error == 0)
		quotes_control(input);
	if (input->dollar > 0 && input->error == 0)
		dollar_expand(input);
	if (input->operator> 0 && input->error == 0)
		operator_control(input);
	if (input->error == 0)
		token_create(input, ft_strlen(input->input), 0, 0);
}
