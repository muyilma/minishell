#include "../libft/libft.h"
#include "../minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	error_and_allocate(t_shell *pro, int exit_code)
{
	ft_executer_free(pro);
	if (pro->env != pro->new_env)
		ft_free(pro->new_env);
	ft_free(pro->env);
	free(pro);
	exit(exit_code);
}

void	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		ft_print_error(NULL, "pwd not found", NULL, 1);
}

void	ft_env(char **args, t_shell *pro)
{
	int	i;

	i = 0;
	if (args[0])
	{
		ft_print_error(NULL, "env: too many arguments", NULL, 1);
		error_and_allocate(pro, 1);
	}
	while (pro->env[i])
	{
		if (ft_strchr(pro->env[i], '='))
			printf("%s\n", pro->env[i]);
		i++;
	}
}
