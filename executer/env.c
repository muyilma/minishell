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

char	**ft_setenv2(char **env, char *variable, int e, int j)
{
	int		i;
	char	*new_var;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], variable, j) == 0 && env[i][j] == '=')
		{
			env[i] = ft_strdup(variable);
			return (env);
		}
		i++;
	}
	if (e == 1)
	{
		new_var = ft_substr(variable, 0, j);
		env[i] = new_var;
	}
	else
	{
		env[i] = ft_strdup(variable);
	}
	env[i + 1] = NULL;
	return (env);
}

char	**ft_setenv(char **env, char *variable, int e)
{
	int		j;
	char	*equal_sign;

	if (!env || !variable)
		return (env);
	equal_sign = ft_strchr(variable, '=');
	if (!equal_sign)
		return (env);
	j = equal_sign - variable;
	return (ft_setenv2(env, variable, e, j));
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
