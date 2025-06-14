/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:40:33 by musyilma          #+#    #+#             */
/*   Updated: 2025/05/27 17:15:08 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"

static void	quotes_control(t_shell *input)
{
	int	i;

	i = 0;
	while (input->input[i])
	{
		if (input->input[i] == 34 || input->input[i] == 39)
		{
			input->quotes++;
			input->qut = input->input[i];
			i++;
			while (input->input[i] != '\0' && input->input[i] != input->qut)
				i++;
			if (input->input[i] == input->qut)
				input->quotes++;
			if (input->input[i] == '\0')
				continue ;
		}
		i++;
	}
	if (input->quotes % 2 != 0)
		input->error = 2;
}

static int	operator_after(t_shell *input, int i, int j, int flag)
{
	while (input->input[++i] && input->input[i] != '|')
	{
		if (input->input[i] == 34 || input->input[i] == 39)
		{
			i = quotes_skip(input->input, i, 1, 0);
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
		if (space_op(input->input[i], 0))
			input->after_str++;
	}
	if (input->after_str == 0)
		input->error = 3;
	return (j);
}

static int	operator_before(t_shell *input, int i)
{
	int	j;
	int	flag;

	flag = 1;
	j = i;
	input->after_str = 0;
	if (input->input[i] == '|' && input->isalpha == 0)
		return (input->error = 3, i);
	if (input->input[i] == '|')
	{
		input->isalpha = 0;
		flag = 0;
	}
	if (input->input[i] == '<')
	{
		if (input->input[i + 1] == '<')
			return (operator_after(input, i + 1, j + 1, flag));
	}
	if (input->input[i] == '>')
	{
		if (input->input[i + 1] == '>')
			return (operator_after(input, i + 1, j + 1, flag));
	}
	return (operator_after(input, i, j, flag));
}

static void	operator_control(t_shell *a)
{
	int	i;

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
		else if (a->input[i] == '|' || a->input[i] == '<' || a->input[i] == '>')
			i = operator_before(a, i);
		else if (space_op(a->input[i], 0))
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
		dollar_expand(input, ft_strlen(input->input));
	if (input->operator > 0 && input->error == 0)
		operator_control(input);
	if (input->error == 0)
		token_parse(input, ft_strlen(input->input), 0, 0);
}
