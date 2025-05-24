#include "../libft/libft.h"
#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>




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
		i = quotes_skip(a->input, i, 0, 0);
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

void	redirect_free(t_shell *input, int i)
{
	if (input->arg[i]->append_outfile)
		free(input->arg[i]->append_outfile);
	if (input->arg[i]->heradock)
		free(input->arg[i]->heradock);
	if (input->arg[i]->infile)
        free(input->arg[i]->infile);
	if (input->arg[i]->outfile)
		free(input->arg[i]->outfile);
}

void	ft_print_error(char *base, char *str, char **arg, int flag)
{
	if (flag == 0)
	{
		write(2, arg[0], ft_strlen(arg[0]));
		write(2, str, ft_strlen(str));
	}
	else if (flag == 1)
		write(2, str, ft_strlen(str));
	else if (flag == 2)
	{
		write(2, base, ft_strlen(base));
		write(2, arg[0], ft_strlen(arg[0]));
		write(2, str, ft_strlen(str));
	}
	write(2, "\n", 2);
}

void	ft_executer_free(t_shell *input)
{
	int	i;
	int	j;

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
			redirect_free(input, i);
			free(input->arg[i]);
			i++;
		}
	}
	free(input->arg);
}
