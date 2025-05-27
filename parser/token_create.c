/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:41:23 by musyilma          #+#    #+#             */
/*   Updated: 2025/05/27 17:14:45 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>

char	**copy_env(char **env, int b)
{
	int		i;
	char	**copy;

	i = 0;
	while (env[i])
		i++;
	copy = malloc(sizeof(char *) * (i + b + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (env[i])
	{
		copy[i] = ft_strdup(env[i]);
		if (!copy[i])
		{
			while (i--)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static int	word_count(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (space_op(str[i], 1))
			i++;
		if (!str[i])
			break ;
		count++;
		while (str[i] && space_op(str[i], 0))
		{
			if (str[i] == 34 || str[i] == 39)
				i = quotes_skip(str, i, 0, 0);
			else
				i++;
		}
	}
	return (count);
}

static void	arg_create(char *str, char **newstr, int i, int j)
{
	int		k;
	char	qut;

	if (!newstr)
		return ;
	k = 0;
	while (j <= i)
	{
		if (str[j] == 34 || str[j] == 39)
		{
			qut = str[j++];
			while (str[j] && str[j] != qut)
				(*newstr)[k++] = str[j++];
			if (str[j])
				j++;
		}
		else
			(*newstr)[k++] = str[j++];
	}
	(*newstr)[k] = '\0';
}

static void	arg_find(char *str, t_shell *ipt, int k, int i)
{
	int	j;
	int	l;
	int	flag;

	l = 0;
	flag = 0;
	while (str[i])
	{
		while (space_op(str[i], 1))
			i++;
		if (!str[i])
			break ;
		j = i;
		while (str[i] && space_op(str[i], 0))
		{
			if (str[i] == 34 || str[i] == 39)
				i = quotes_skip(str, i, 1, &flag);
			i++;
		}
		ipt->arg[k]->str[l] = malloc((i - j) - flag + 1);
		flag = 0;
		arg_create(str, &ipt->arg[k]->str[l], i - 1, j);
		l++;
	}
	ipt->arg[k]->str[l] = NULL;
}

void	token_create(t_shell *ipt, char *str, int k)
{
	int	count;

	if (!str)
		return ;
	count = word_count(str);
	ipt->arg[k]->str = malloc(sizeof(char *) * (count + 1));
	if (!ipt->arg[k]->str)
		return ;
	ipt->arg[k]->str[count] = NULL;
	arg_find(str, ipt, k, 0);
}
