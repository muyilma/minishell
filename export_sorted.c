#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>


char	**ft_copy_env(char **env, int count)
{
	int		i;
	char	**sorted_env;

	sorted_env = malloc(sizeof(char *) * (count + 1));
	if (!sorted_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		sorted_env[i] = ft_strdup(env[i]);
		if (!sorted_env[i])
		{
			while (--i >= 0)
				free(sorted_env[i]);
			free(sorted_env);
			return (NULL);
		}
		i++;
	}
	sorted_env[i] = NULL;
	return (sorted_env);
}

void	ft_sort_array(char **sorted_env, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strncmp(sorted_env[j], sorted_env[j + 1],
					ft_strlen(sorted_env[j])) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}
void	ft_print_sorted_env(char **env)
{
	int		i;
	int		count;
	char	**sorted_env;

	if (!env)
		return ;
	count = 0;
	while (env[count])
		count++;
	sorted_env = ft_copy_env(env, count);
	if (!sorted_env)
		return ;
	ft_sort_array(sorted_env, count);
	i = 0;
	while (sorted_env[i])
	{
		printf("declare -x %s\n", sorted_env[i]);
		i++;
	}
	i = 0;
	while (sorted_env[i])
	{
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

