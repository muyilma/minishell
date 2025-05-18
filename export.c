#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	ft_export_parser(char *args)
{
	int	i;

	i = 0;
	if (args[i] == '=')
		return (1);
	if (args[0] >= '0' && args[0] <= '9')
		return (1);
	while (args[i] && (args[i] != '=' || (args[i] == '=' && i == 0)))
	{
		if (!((args[i] >= 'a' && args[i] <= 'z') || 
			(args[i] >= 'A' && args[i] <= 'Z') ||
			(args[i] >= '0' && args[i] <= '9' && i > 0) ||
			args[i] == '_' || args[i] == '='))
		{
			return (1);
		}
		i++;
	}
	return (0);
}
void	ft_cmdcess_export_arg2(char *arg, t_shell *pro, char	*variable)
{
	if (!ft_getenv(pro->env, arg))
	{
		variable = ft_strjoin(arg, "=");
		if (variable)
		{
			pro->env = ft_setenv(pro->env, variable, 1);
			free(variable);
		}
	}
}
int	ft_cmdcess_export_arg(char *arg, t_shell *pro, int envflag)
{
	char	*equal_sign;
	char	*variable;

	if ((ft_export_parser(arg) == 1) && envflag == 0)
	{
		ft_print_error("minishell: export: ", " : not a valid identifier", &arg, 2);
		return (1);
	}
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		variable = ft_strdup(arg);
		if (variable)
		{
			pro->env = ft_setenv(pro->env, variable, 0);
			free(variable);
		}
	}
	else
	{
		ft_cmdcess_export_arg2(arg, pro,variable);
	}
	return (0);
}

int	ft_export(char **args, t_shell *pro , int envflag)
{
	int	i;
	int ex;
	i = 0;
	if (!args || !args[0])
	{
		while (pro->env[i])
		{
			printf("declare -x %s\n", pro->env[i]);
			i++;
		}
		return (0);
	}
	while (args[i])
	{
		ex = ft_cmdcess_export_arg(args[i], pro, envflag);
		i++;
	}
	return (ex);
}
