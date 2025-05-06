#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	ft_echo(char **args)
{
	int	i;
	int	newline;

	newline = 1;
	i = 0;
	if (args[0] && ft_strncmp(args[0], "-n", 3) == 0)
	{
		newline = 0;
		i = 1;
	}
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

void	ft_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

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
void	ft_export(char **args, t_input *pro)
{
	int		i;
	char	*equal_sign;
	char	*name;
	char	*value;
	char	*variable;

	if (!args || !args[0])
	{
		ft_print_sorted_env(pro->env);
		return ;
	}
	i = 0;
	while (args[i])
	{
		// '=' işareti var mı kontrol et
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			// NAME=VALUE formatında - ekle veya güncelle
			// Orijinal stringi doğrudan kullan, ft_setenv içinde kopyalanacak
			variable = ft_strdup(args[i]);
			if (variable)
			{
				pro->env = ft_setenv(pro->env, variable);
				free(variable);
			}
		}
		else
		{
			// Sadece NAME formatında - yalnızca değişken yoksa ekle
			if (!ft_getenv(pro->env, args[i]))
			{
				variable = ft_strjoin(args[i], "=");
				if (variable)
				{
					pro->env = ft_setenv(pro->env, variable);
					free(variable);
				}
			}
		}
		i++;
	}
}

// built_in fonksiyonunu güncelle
int	built_in(char **args, t_input *pro)
{
	if (ft_strncmp(args[0], "echo", 5) == 0)
	{
		ft_echo(&args[1]);
		exit(0);
	}

	if (ft_strncmp(args[0], "env", 4) == 0)
	{
		ft_env(pro->env);
		return (1);
	}

	if (ft_strncmp(args[0], "export", 7) == 0)
	{
		ft_export(&args[1], pro);
		return (1);
	}
	return (0);
}