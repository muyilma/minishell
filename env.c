#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

char	*ft_getenv(char **env, char *name)
{
	int	i;
	int	len;

	i = 0;
	if (!env || !name)
		return (NULL);
	len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	**ft_setenv(char **env, char *variable)
{
	int		i;
	int		j;
	char	*name;
	char	*equal_sign;
	char	**new_env;

	if (!env || !variable)
		return (env);
	equal_sign = ft_strchr(variable, '=');
	if (!equal_sign)
		return (env);
	j = equal_sign - variable;
	name = malloc(j + 1);
	if (!name)
		return (env);
	ft_strlcpy(name, variable, j + 1);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, j) == 0 && env[i][j] == '=')
		{
			env[i] = ft_strdup(variable);
			free(name);
			return (env);
		}
		i++;
	}
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
	{
		free(name);
		return (env);
	}
	j = 0;
	while (j < i)
	{
		new_env[j] = env[j];
		j++;
	}
	new_env[i] = ft_strdup(variable);
	new_env[i + 1] = NULL;
    j = 0;
    while (new_env[j])
	{
        printf("%s\n",new_env[j]);
		j++;
	}
	free(name);
	return (new_env);
}

void	ft_free(char **str)
{
	int	i;

	i = 0;
	while (str[i] != 0)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

char	*pathc(char *cmd, char **envp)
{
	int i;
	char **path;
	char *arg;
	char *str;
	char *path_value;

	path_value = ft_getenv(envp, "PATH");
	if (!path_value)
		return (NULL);

	path = ft_split(path_value, ':');
	i = -1;
	while (path[++i])
	{
		arg = ft_strjoin(path[i], "/");
		str = ft_strjoin(arg, cmd);
		free(arg);
		if (access(str, X_OK) == 0)
		{
			ft_free(path);
			return (str);
		}
		free(str);
	}
	ft_free(path);
	return (NULL);
}