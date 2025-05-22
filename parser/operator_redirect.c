#include "../libft/libft.h"
#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	redirect_control(char *redirect, int flag)
{
	int	fd;

	if (flag == -1)
		return (0);
	if (flag == 0)
	{
		if (access(redirect, R_OK) == -1)
			return (1);
		fd = open(redirect, O_RDONLY);
		if (fd == -1)
			return (1);
	}
	else if (flag == 1 || flag == 2)
	{
		if (flag == 1)
			fd = open(redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(redirect, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			return (1);
		if (access(redirect, W_OK) == -1)
			return (1);
	}
	return (0);
}

char	*strcut(char *str, int start, int *end, int size)
{
	int		i;
	int		j;
	char	*newstr;

	if (size <= 0)
		return (NULL);
	newstr = malloc(size + 1);
	if (!newstr)
		return (NULL);
	i = -1;
	j = 0;
	while (str[++i])
	{
		if (i >= start && i <= *end)
			continue ;
		newstr[j] = str[i];
		j++;
	}
	newstr[j] = '\0';
	free(str);
	*end = -1;
	return (newstr);
}

char	*redirect_create(char *s, int start, int len, int flag)
{
	char	*a;
	int		i;
	char	qut;
	int		j;

	j = 0;
	i = 0;
	a = malloc((len + 1) - flag);
	if (a == NULL)
		return (NULL);
	while (i < len)
	{
		if (s[start + i] == 34 || s[start + i] == 39)
		{
			qut = s[start + i++];
			while (s[start + i] && s[start + i] != qut)
				a[j++] = s[start + i++];
			if (s[start + i] == qut)
				i++;
		}
		else
			a[j++] = s[start + i++];
	}
	a[j] = '\0';
	return (a);
}

char	*redirect_skip(char **redirect, char *str, int *t, int *flag)
{
	int		j;
	int		k;
	int		quotes;
	char	*temp;

	quotes = 0;
	j = *t;
	(*t)++;
	if (str[*t] == '<' || str[*t] == '>')
		(*t)++;
	while (str[*t] == ' ')
		(*t)++;
	k = *t;
	while (str[*t] != ' ' && str[*t] != '\0')
	{
		if (str[*t] == 34 || str[*t] == 39)
			*t = quotes_skip(str, *t, 1, &quotes);
		(*t)++;
	}
	temp = *redirect;
	*redirect = redirect_create(str, k, (*t) - k, quotes);
	free(temp);
	*flag = redirect_control(*redirect, *flag);
	return (strcut(str, j, t, ft_strlen(str) - (*t - j)));
}

char	*redirect_find(char **redirect, char *str, int *i, int *flag)
{
	if (str[*i] == '<' && str[*i + 1] == '<')
	{
		*flag = -1;
		str = redirect_skip(redirect, str, i, flag);
		*flag = 0;
	}
	else if (str[*i] == '<')
		str = redirect_skip(redirect, str, i, flag);
	else if (str[*i] == '>' && str[*i + 1] == '>')
	{
		*flag = 2;
		str = redirect_skip(redirect, str, i, flag);
	}
	else if (str[*i] == '>')
	{
		*flag = 1;
		str = redirect_skip(redirect, str, i, flag);
	}
	
	return (str);
}
