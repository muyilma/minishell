#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int is_valid_n_flag(char *str)
{
    int i = 1;

    if (!str || str[0] != '-' || str[1] == '\0')
        return 0;

    while (str[i])
    {
        if (str[i] != 'n')
            return 0;
        i++;
    }
    return 1;
}

void ft_echo2(char **args, int i, int newline)
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

void ft_echo(char **args)
{
    int i = 0;
    int newline = 1;

    while (args[i] && is_valid_n_flag(args[i]))
    {
        newline = 0;
        i++;
    }

    ft_echo2(args, i, newline);
}

void	ft_unset2(t_shell *pro, int j)
{
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
			if (ft_strncmp(pro->env[j], args[i], len) == 0 && 
			    pro->env[j][len] == '=')
			{
				ft_unset2(pro, j);
				break;
			}
			j++;
		}
		i++;
	}
	return (0);
}
