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

void	ft_env(char **env, char *args)
{
	int	i;

	i = 0;
	if (args)
	{
		perror("env hatası");
		return ;
	}
		
	while (env[i])
	{
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
		ft_env(pro->env, args[1]);
		exit(0);
	}

	if (ft_strncmp(args[0], "export", 7) == 0)
	{
		ft_export(&args[1], pro);
		return (1);
	}

	if (ft_strncmp(args[0], "unset", 7) == 0)
	{
		ft_export(&args[1], pro);
		return (1);
	}
	return (0);
}