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

void	ft_unset(char **args, t_input *pro)
{
	int	i;
	int	j;
	int	len;
	
	if (!args || !pro->env)
		return ;
	
	i = 0;
	while (args[i])
	{
		len = ft_strlen(args[i]);
		j = 0;
		while (pro->env[j])
		{
			if (ft_strncmp(pro->env[j], args[i], len) == 0 && 
			    pro->env[j][len] == '=')
			{
				while (pro->env[j + 1])
				{
					pro->env[j] = pro->env[j + 1];
					j++;
				}
				pro->env[j] = NULL;
				break;
			}
			j++;
		}
		i++;
	}
}

void	ft_env(char **env, char **args, t_input *pro)
{
	int	i;

	i = 0;
	if (args && args[0])
	{
		if (ft_strchr(args[0], '=') == NULL)
		{
			perror("env:");
			exit(0);
		}
		ft_export(&args[0], pro, 1);
	}
	while (env[i])
	{
		if(ft_strchr(env[i], '='))
			printf("%s\n", env[i]);
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
		ft_env(pro->env, &args[1], pro);
		exit(0);
	}

	if (ft_strncmp(args[0], "export", 6) == 0)
	{
		ft_export(&args[1], pro, 0);
		return (1);
	}

	if (ft_strncmp(args[0], "unset", 5) == 0)
	{
		ft_unset(&args[1], pro);
		return (1);
	}
	return (0);
}