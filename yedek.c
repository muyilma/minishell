#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>


char	*strcut(char *str, int start, int end, int size)
{
	int i;
	int j;
	char *newstr;

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
	int j;
	int k;
	int i;

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
		{
			i = quotes_skip(str, i);
			i--;
		}
		i++;
	}
	*redirect = ft_substr(str, k, i - k);
	str = strcut(str, j, i, ft_strlen(str) - (i - j));
	*t = -1;
	return (str);
}

char	*redirect_convert(t_input *ipt, char *str, int k)
{
	int i;

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

int	arg_parse2(t_input *ipt, int i, int j, int k)
{
	char *fakestr;
	char *temp;

	if (ipt->input[i + 1] == '\0')
		i++;
	fakestr = ft_substr(ipt->input, j, i - j);
	ipt->arg[k] = ft_calloc(1, sizeof(t_pro));
	temp = fakestr;
	fakestr = redirect_convert(ipt, fakestr, k);
	// if (temp !=fakestr)
	// 	free(temp);
	arg_convert(ipt, fakestr, k);
	free(fakestr);
	return (i);
}

void	arg_parse(t_input *ipt, int len, int k)
{
	int i;
	int j;

	i = -1;
	j = 0;
	ipt->arg = malloc(sizeof(t_pro *) * (ipt->pipe + 2)); // burası +1di
	if (!ipt->arg)
		return ;
	while (ipt->input[++i] && i < len)
	// len'i koymayınca fazladan 1 kere daha giriyor
	{
		if (ipt->input[i] == 34 || ipt->input[i] == 39)
		{
			i = quotes_skip(ipt->input, i);
			i--;
		}
		if (ipt->input[i] == '\0' || ipt->input[i] == '|' || (ipt->input[i+ 1] == '\0'))
		{
			i = arg_parse2(ipt, i, j, k);
			k++;
			j = i + 1;
		}
	}
	ipt->arg[ipt->pipe + 1] = NULL;
}









void	arg_find(char *str, t_input *ipt, int k)
{
	int	i;
	int	j;
	int	l;
	int	flag;

	i = 0;
	l = 0;
	flag = 0;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break ;
		j = i;
		while (str[i] && str[i] != ' ')
		{
			if (str[i] == 34 || str[i] == 39)
			{
				flag += 2;
				i = quotes_skip(str, i);
			}
			else
				i++;
		}
		ipt->arg[k]->str[l] = malloc((i - j) - flag + 1);
		flag = 0;
		arg_create(str, &ipt->arg[k]->str[l], i -1, j);
		l++;
	}
	ipt->arg[k]->str[l] = NULL;
}

void	arg_convert(t_input *ipt, char *str, int k)
{
	int count;
	int	i;
	int	j;
	int	l;
	int	flag;

	i = 0;
	l = 0;
	flag = 0;
	if (!str)
		return ;
	count = word_count(str);
	ipt->arg[k]->str = malloc(sizeof(char *) * (count + 1));
	ipt->arg[k]->str[count] = NULL;
	if (!ipt->arg[k]->str)
		return ;
    
	arg_find(str, ipt, k);
}
