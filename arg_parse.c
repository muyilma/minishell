#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

char	*strcut(char *str, int start, int end, int size)
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
		if (i >= start && i <= end)
			continue ;
		newstr[j] = str[i];
		j++;
	}
	newstr[j] = '\0';
	free(str);
	return (newstr);
}

char	*redirect_skip(char **redirect, char *str, int *t)
{
	int	j;
	int	k;
	int	i;

	i = *t;
	j = i;
	i++;
	if (str[i] == '<' || str[i] == '>')
		i++;
	while (str[i] == ' ')
		i++;
	k = i;
	while (str[i] != ' ' && str[i] != '\0')
	{
		if (str[i] == 34 || str[i] == 39)
			i = quotes_skip(str, i);
		i++;
	}
	*redirect = ft_substr(str, k, i - k);
	str = strcut(str, j, i, ft_strlen(str) - (i - j));
	*t = -1;
	return (str);
}

char	*redirect_convert(t_input *ipt, char *str, int k)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == 34 || str[i] == 39)
			i = quotes_skip(str, i);
		if (str[i] == '<' && str[i + 1] == '<')
			str = redirect_skip(&ipt->arg[k]->heradock, str, &i);
		else if (str[i] == '<')
			str = redirect_skip(&ipt->arg[k]->infile, str, &i);
		else if (str[i] == '>' && str[i + 1] == '>')
			str = redirect_skip(&ipt->arg[k]->append_outfile, str, &i);
		else if (str[i] == '>')
			str = redirect_skip(&ipt->arg[k]->outfile, str, &i);
		if (!str)
			return (NULL);
	}
	return (str);
}



void	arg_parse(t_input *ipt, int len, int k)
{
	int		i;
	int		j;
	char	*fakestr;
	char	*temp;

	if (ipt->error > 0 || !ipt->input)
		return ;
	i = -1;
	j = 0;
	ipt->arg = malloc(sizeof(t_pro *) * (ipt->pipe + 1));
	if (!ipt->arg)
		return ;
	while (ipt->input[++i] && i < len)
	// len'i koymayınca fazladan 1 kere daha giriyor
	{
		if (ipt->input[i] == '|' || (ipt->input[i + 1] == '\0'))
		{
			if (ipt->input[i + 1] == '\0')
				i++;
			fakestr = ft_substr(ipt->input, j, i - j);
			ipt->arg[k] = malloc(sizeof(t_pro));
			ipt->arg[k]->append_outfile=NULL;
			ipt->arg[k]->heradock=NULL;
			ipt->arg[k]->infile=NULL;
			ipt->arg[k]->outfile=NULL;
			ipt->arg[k]->str=NULL;
			if (!ipt->arg[k])
				return ;
			temp=fakestr;
			fakestr = redirect_convert(ipt, fakestr, k);
			free(temp);
			arg_convert(ipt, fakestr, k);
			k++;
			free(fakestr);
			j = i + 1;
		}
	}
	ipt->arg[k] = NULL;
}
