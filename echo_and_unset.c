#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
void ft_echo(char **args, int i, int newline) 
{
    int j;
    int valid_n_flag;

    if (args[0] && args[0][0] == '-') 
	{
        j = 1;
        valid_n_flag = 1;
        while (args[0][j]) 
		{
            if (args[0][j] != 'n')
			{
                valid_n_flag = 0;
                break;
            }
            j++;
        }
        if (valid_n_flag && j > 1) 
		{
            newline = 0;
            i = 1;
        }
    }
    ft_echo2(args, i, newline);
}

void	ft_unset2(t_input *pro, int j)
{
    while (pro->env[j + 1])
	{
		pro->env[j] = pro->env[j + 1];
		j++;
	}
	pro->env[j] = NULL;
}

int	ft_unset(char **args, t_input *pro)
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
