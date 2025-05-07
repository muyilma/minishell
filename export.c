#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	ft_print_sorted_env(char **env)
{
	int		i;
	int		j;
	int		count;
	char	**sorted_env;
	char	*temp;

	if (!env)
		return ;
	count = 0;
	while (env[count])
		count++;
	sorted_env = malloc(sizeof(char *) * (count + 1));
	if (!sorted_env)
		return ;
	i = 0;
	while (i < count)
	{
		sorted_env[i] = ft_strdup(env[i]);
		if (!sorted_env[i])
		{
			while (--i >= 0)
				free(sorted_env[i]);
			free(sorted_env);
			return ;
		}
		i++;
	}
	sorted_env[i] = NULL;
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strncmp(sorted_env[j], sorted_env[j + 1],
					ft_strlen(sorted_env[j])) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (sorted_env[i])
	{
		printf("declare -x %s\n", sorted_env[i]);
		i++;
	}
	i = 0;
	while (sorted_env[i])
	{
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}
int	ft_export_parser(char *args)
{
	int	i;

	i = 0;
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
void	ft_export(char **args, t_input *pro)
{
	int i;
	char *equal_sign;
	char *name;
	char *value;
	char *variable;

	if (!args || !args[0])
	{
		ft_print_sorted_env(pro->env);
		return ;
	}
	i = 0;
	while (args[i])
	{
        if (ft_export_parser(args[i]) == 1)
		{
			perror("export error");
			i++;
			continue;
		}
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			variable = ft_strdup(args[i]);
			if (variable)
			{
				pro->env = ft_setenv(pro->env, variable, 0);
				free(variable);
			}
		}
		else
		{
			if (!ft_getenv(pro->env, args[i]))
			{
				variable = ft_strjoin(args[i], "=");
				if (variable)
				{
					pro->env = ft_setenv(pro->env, variable, 1);
					free(variable);
				}
			}
		}
		i++;
	}
}