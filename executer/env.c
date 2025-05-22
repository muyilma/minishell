#include "../libft/libft.h"
#include "../minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
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

char	**ft_setenv2(char **env, char *variable,int j)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], variable, j) == 0 && env[i][j] == '=')
		{
			new_env = copy_env(env, 0);
			free(new_env[i]);
			new_env[i] = ft_strdup(variable);
			ft_free(env);
			return (new_env);
		}
		i++;
	}
	new_env = copy_env(env, 1);
	new_env[i] = ft_strdup(variable);
	new_env[i + 1] = NULL;
	ft_free(env);
	return (new_env);
}

char	**ft_setenv(char **env, char *variable)
{
	int		j;
	char	*equal_sign;
	if (!env || !variable)
		return (env);
	equal_sign = ft_strchr(variable, '=');
	if (!equal_sign)
		return (env);
	j = equal_sign - variable;
	return (ft_setenv2(env, variable,j));
}

void	ft_free(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
	str = NULL;
}

char	*pathc(char *cmd, char **envp, int i)
{
	char	**path;
	char	*arg;
	char	*str;
	char	*path_value;

	path_value = ft_getenv(envp, "PATH");
	if (!path_value)
		return (NULL);
	path = ft_split(path_value, ':');
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
