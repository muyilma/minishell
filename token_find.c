#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


char	*redirect_convert(t_shell *ipt, char *str, int k, int *flag)
{
	int i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == 34 || str[i] == 39)
			i = quotes_skip(str, i, 0);
		if (str[i] == '<' && str[i + 1] == '<')
			str = redirect_find(&ipt->arg[k]->heradock, str, &i, flag);
		else if (str[i] == '<')
			str = redirect_find(&ipt->arg[k]->infile, str, &i, flag);
		else if (str[i] == '>' && str[i + 1] == '>')
			str = redirect_find(&ipt->arg[k]->append_outfile, str, &i, flag);
		else if (str[i] == '>')
			str = redirect_find(&ipt->arg[k]->outfile, str, &i, flag);
		if (!str)
			return (NULL);
		if (*flag != 0)
			return (NULL);
	}
	return (str);
}

int	token_find(t_shell *ipt, int i, int j, int k)
{
	char *fakestr;
	char *temp;
	int flag;

	flag = 0;
	if (ipt->input[i + 1] == '\0')
		i++;
	fakestr = ft_substr(ipt->input, j, i - j);
	ipt->arg[k] = ft_calloc(1, sizeof(t_cmd));
	if (!ipt->arg[k])
		return (0);
	temp = fakestr;
	fakestr = redirect_convert(ipt, fakestr, k, &flag);
	// if (temp !=fakestr)
	// 	free(temp);
	if (flag == 0)
		arg_convert(ipt, fakestr, k);
	free(fakestr);
	return (i);
}

void	token_create(t_shell *ipt, int len, int k,int flag)
{
	int i;
	int j;

	i = -1;
	j = 0;
	ipt->arg = malloc(sizeof(t_cmd *) * (ipt->pipe + 2)); // burası +1di
	if (!ipt->arg)
		return ;
	while (ipt->input[++i] && i < len)// len'i koymayınca fazladan 1 kere daha giriyor
	{
		flag++;
		if (ipt->input[i] == 34 || ipt->input[i] == 39)
			i = quotes_skip(ipt->input, i, 1);
		if (ipt->input[i] == '\0' || ipt->input[i] == '|' || ipt->input[i
			+ 1] == '\0')
		{
			i = token_find(ipt, i, j, k);
			k++;
			j = i + 1;
		}
	}
	if (flag == 0)
		ipt->arg[ipt->pipe] = NULL;
	else
		ipt->arg[ipt->pipe + 1] = NULL;
}
