#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

int	quadot_skip(char *str, int j) // bunuda farklı bir yerde tanımla
{
	int i;
	char qut;

	i = j;
	if (str[i] == 34 || str[i] == 39)
	{
		qut = str[i++];
		while (str[i] && str[i] != qut)
			i++;
		if (str[i])
			i++;
	}
	return (i);
}

int	pipe_count(char *str) // bu fonksiyona gerek yok emptylineda yap
{
	int i;
	int count;

	i = -1;
	count = 0;

	while (str[++i])
	{
		if (str[i] == 34 || str[i] == 39)
			i = quadot_skip(str, i);

		if (str[i] == '|')
			count++;
	}
	return (count);
}

char	*strcut(char *str, int start, int end, int size)
{
	int		i;
	int		j;
	char	*newstr;

	newstr = malloc(size + 1);
	i = -1;
	j = 0;
	while (str[++i])
	{
		if (i == start)
		{
			while (i <= end)
				i++;
		}
		newstr[j] = str[i];
		j++;
	}
	newstr[j] = '\0';
	return (newstr);
}

char	*redirect_skip(char **redirect, char *str, int i)
{
	int	j;
	int	k;

	j = i;
	i++;
	while (str[i] == ' ')
		i++;
	k = i;
	while (str[i] != ' ')
	{
		if (str[i] == 34 || str[i] == 39)
			i = quadot_skip(str, i);
		i++;
	}
	*redirect = ft_substr(str, k, i - k);
	str = strcut(str, j, i, ft_strlen(str) - (i - j));
	return (str);
}

char	*redirect_convert(t_input *ipt, char *str, int k)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i]== 34 || str[i]== 39)
		i = quadot_skip(ipt->input, i);
		if (str[i]== '<' && ipt->input[i + 1] == '<')
		{
			str = redirect_skip(&ipt->arg[k]->heradock, str, i);
			i = -1;
		}
		else if (str[i]== '<')
		{
			str = redirect_skip(&ipt->arg[k]->infile, str, i);
			i = -1;
		}
		else if (str[i]== '>' && ipt->input[i + 1] == '>')
		{
			str = redirect_skip(&ipt->arg[k]->append_outfile, str, i);
			i = -1;
		}
		else if (str[i]== '>')
		{
			str = redirect_skip(&ipt->arg[k]->outfile, str, i);
			i = -1;
		}
	}
	return str;
}
void	arg_parse(t_input *ipt, int len, int k)
{
	int		i;
	int		j;
	char	*fakestr;

	if (ipt->error > 0)
		return ;
	i = -1;
	j = 0;
	ipt->arg = malloc(sizeof(t_pro *) * pipe_count(ipt->input));
	while (ipt->input[++i] && i < len)
	// len'i koymayınca fazladan 1 kere daha giriyor
	{
		if (ipt->input[i] == 34 || ipt->input[i] == 39)
			i = quadot_skip(ipt->input, i);
		if (ipt->input[i] == '|' || (ipt->input[i + 1] == '\0'))
		{
			if (ipt->input[i + 1] == '\0')
				i++;
			fakestr = ft_substr(ipt->input, j, i - j);
			ipt->arg[k] = malloc(sizeof(t_pro));
			fakestr = redirect_convert(ipt, fakestr, k);
			k++;
			printf("%s\n", fakestr);
			free(fakestr);
			j = i + 1;
		}
	}
}
