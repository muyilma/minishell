#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

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

void	built_in3(char **args, t_input *pro)
{
	if (ft_strncmp(args[0], "unset", 5) == 0)
	{
		ft_unset(&args[1], pro);
		exit(0);
	}
	if (ft_strncmp(args[0], "export", 7) == 0)
	{
		ft_export(&args[1], pro, 0);
		exit(0);
	}
	if (ft_strncmp(args[0], "pwd", 4) == 0)
	{
		ft_pwd();
		exit(0);
	}
}

void	built_in(char **args, t_input *pro)
{
	if (ft_strncmp(args[0], "echo", 5) == 0)
	{
		ft_echo(&args[1], 0, 1);
		exit(0);
	}
	if (ft_strncmp(args[0], "env", 4) == 0)
	{
		ft_env(pro->env, &args[1], pro);
		exit(0);
	}
	if (ft_strncmp(args[0], "exit", 5) == 0)
    {
        ft_exit(&args[1]);
        exit(0);
    }
	if (ft_strncmp(args[0], "cd", 3) == 0)
    {
        ft_cd(&args[1], pro);
        exit(0);
    }
	built_in3(args, pro);
}


int	built_in2(char **args, t_input *pro)
{
	if (!args || !args[0])
		return (1);
	if (ft_strncmp(args[0], "exit", 5) == 0)
    {
        ft_exit(&args[1]);
        return (1);
    }
	if (ft_strncmp(args[0], "cd", 3) == 0)
    {
        ft_cd(&args[1], pro);
        return (1);
    }
	if (ft_strncmp(args[0], "unset", 5) == 0)
		return (ft_unset(&args[1], pro));

	if (ft_strncmp(args[0], "export", 7) == 0)
		return (ft_export(&args[1], pro, 0));
}
