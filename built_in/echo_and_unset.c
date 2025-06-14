/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_and_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:37:08 by musyilma          #+#    #+#             */
/*   Updated: 2025/05/27 18:10:09 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

static int	is_valid_n_flag(char *str)
{
	int	i;

	i = 1;
	if (!str || str[0] != '-' || str[1] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static void	ft_echo2(char **args, int i, int newline)
{
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}

void	ft_echo(char **args)
{
	int	i;
	int	newline;

	i = 0;
	newline = 1;
	while (args[i] && is_valid_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	ft_echo2(args, i, newline);
}

static void	ft_unset2(t_shell *pro, int j)
{
	free(pro->env[j]);
	while (pro->env[j + 1])
	{
		pro->env[j] = pro->env[j + 1];
		j++;
	}
	pro->env[j] = NULL;
}

int	ft_unset(char **args, t_shell *pro)
{
	int	i;
	int	j;
	int	len;

	if (!args || !pro->env)
		return (1);
	i = 0;
	while (args[i])
	{
		len = ft_strlen(args[i]);
		j = 0;
		while (pro->env[j])
		{
			if (ft_strncmp(pro->env[j], args[i], len) == 0
				&& pro->env[j][len] == '=')
			{
				ft_unset2(pro, j);
				break ;
			}
			j++;
		}
		i++;
	}
	return (0);
}
