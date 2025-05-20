#include "../libft/libft.h"
#include "../minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		ft_print_error(NULL, "pwd not found", NULL, 1);
}

void	ft_env(char **env, char **args, t_shell *pro)
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
		if (ft_strchr(env[i], '='))
			printf("%s\n", env[i]);
		i++;
	}
}
