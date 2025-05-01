#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

static int	word_count(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (!str[i])
			break ;
		count++;
		while (str[i] && str[i] != ' ')
		{
			if (str[i] == 34 || str[i] == 39)
				i = quotes_skip(str, i);
			else
				i++;
		}
	}
	return (count);
}

void	arg_create(char *str, char **newstr, int i, int j)
{
	int		k;
	char	qut;

	k = 0;
	while (j <= i)
	{
		if (str[j] == 34 || str[j] == 39)
		{
			qut = str[j++];
			while (str[j] && str[j] != qut)
			{
				(*newstr)[k] = str[j];
				j++;
				k++;
			}
			if (str[j])
				j++;
		}
		else
		{
			(*newstr)[k] = str[j];
			// printf("--%c---\n",(*newstr)[k]);
			k++;
			j++;
		}
	}
	(*newstr)[k] = '\0';
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
		// printf("%d\n",(i - j) - flag +1);
		flag = 0;
		arg_create(str, &ipt->arg[k]->str[l], i, j);
		l++;
	}
	ipt->arg[k]->str[l] = NULL;
}

void	arg_convert(t_input *ipt, char *str, int k)
{
	int count;

	if (!str)
		return ;
	count = word_count(str);
	ipt->arg[k]->str = malloc(sizeof(char *) * (count + 1));
	ipt->arg[k]->str[count] = NULL;
	if (!ipt->arg[k]->str)
		return ;
	arg_find(str, ipt, k);
}