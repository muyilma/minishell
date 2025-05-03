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
	int i;
	int newline;

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
void	ft_env(char **environ)
{
	int i;

	i = 0;
	while (environ[i])
	{
		printf("%s\n",environ[i]);
		i++;
	}
}

// void	ft_export(char**args, char **environ)
// {
// 	int i;

// 	i = 0;
// 	if(getenv(args))
// 	{
// 		setenv()
// 	}
// }

void built_in(char **args, char **environ)
{
	if (ft_strncmp(args[0], "echo", 5) == 0)
	{
		ft_echo(&args[1]);
		exit(0);
	}

	if (ft_strncmp(args[0], "env", 4) == 0)
	{
		ft_env(environ);
		exit(0);
	}

	// if (ft_strncmp(args[0], "export", 7) == 0)
	// {
	// 	ft_export(&args[1], e);
	// 	exit(0);
	// }
}
