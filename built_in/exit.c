/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:37:32 by musyilma          #+#    #+#             */
/*   Updated: 2025/05/27 16:54:20 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int	is_longlong_overflow(char *str)
{
	int					i;
	int					sign;
	unsigned long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		if ((sign == 1 && result > MAX_SIGNED_LONG_LONG) || (sign == -1
				&& result > (unsigned long long)MAX_SIGNED_LONG_LONG + 1))
			return (1);
		i++;
	}
	return (0);
}

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args, t_shell *pro)
{
	int	exit_status;

	printf("exit\n");
	if (!args || !args[0])
		error_and_allocate(pro, 0);
	exit_status = ft_atoi(args[0]);
	if (args[1])
	{
		ft_print_error(NULL, "exit: too many arguments", NULL, 1);
		return (1);
	}
	if (!is_numeric(args[0]) || is_longlong_overflow(args[0]))
	{
		ft_print_error("minishell: exit:", ": numeric argument required", args,
			2);
		error_and_allocate(pro, 2);
	}
	error_and_allocate(pro, exit_status);
	return (1);
}
