/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_and_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:37:13 by musyilma          #+#    #+#             */
/*   Updated: 2025/05/28 10:34:16 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	space_op(int c, int flag)
{
	if (flag == 0)
	{
		if ((c >= 9 && c <= 13) || c == ' ')
			return (0);
		return (1);
	}
	if (flag == 1)
	{
		if ((c >= 9 && c <= 13) || c == ' ')
			return (1);
	}
	return (0);
}

void	error_and_allocate(t_shell *pro, int exit_code)
{
	ft_executer_free(pro);
	ft_free(pro->env);
	free(pro);
	set_exit_status_code(exit_code);
	exit(*get_exit_status_code());
}

void	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		ft_print_error(NULL, "pwd not found", NULL, 1);
}

void	ft_env(char **args, t_shell *pro)
{
	int	i;

	i = 0;
	if (args[0])
	{
		ft_print_error(NULL, "env: too many arguments", NULL, 1);
		error_and_allocate(pro, 1);
	}
	while (pro->env[i])
	{
		if (ft_strchr(pro->env[i], '='))
			printf("%s\n", pro->env[i]);
		i++;
	}
}
