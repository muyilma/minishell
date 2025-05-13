#include "libft/libft.h"
#include "minishell.h"
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
		if (str[i] == qut)
			i++;
	}
	return (i);
}

void	quotes_control(t_input *input)
{
	int	i;

	i = -1;
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
}

void	redirect_free(t_input *input, int i)
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

			redirect_free(input, i);
			free(input->arg[i]);
			i++;
		}
	}
	free(input->arg);
}
