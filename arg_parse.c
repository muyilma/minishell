#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int	redirect_control(char *redirect, int flag)
{
	int fd;

	if (flag == 0)
	{
		if (access(redirect, R_OK) == -1)
			return (1);

		fd = open(redirect, O_RDONLY);
		if (fd == -1)
			return (1);
		close(fd);
	}
	else if (flag == 1)
	{
		fd = open(redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (1);

		if (access(redirect, W_OK) == -1)
		{
			close(fd);
			return (1);
		}
		close(fd);
	}
	return (0);
}

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
char	*redirect_create(char *s, unsigned int start, int len, int flag)
{
	char *a;
	unsigned int i;
	char qut;
	int j;

	j = 0;
	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_calloc(sizeof(char), 1));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	a = malloc((len * sizeof(char) + 1) - flag);
	if (a == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		if (s[start + i] == 34 || s[start + i] == 39)
		{
			qut = s[start + i];
			i++;
			while (s[start + i] && s[start + i] != qut)
			{
				a[j] = s[start + i];
				j++;
				i++;
			}
			if (s[start + i] == qut)
				i++;
		}
		else
		{
			a[j] = s[start + i];
			j++;
			i++;
		}
	}
	a[i] = '\0';
	return (a);
}

char	*redirect_skip(char **redirect, char *str, int *t, int *flag)
{
	int j;
	int k;
	int i;
	int quotes;

	quotes = 0;
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
			quotes += 2;
			i--;
		}
		i++;
	}
	*redirect = redirect_create(str, k, i - k, *flag);
	if (*flag != -1)
		*flag = redirect_control(*redirect, *flag);
	str = strcut(str, j, i, ft_strlen(str) - (i - j));
	*t = -1;
	return (str);
}

char	*redirect_convert(t_input *ipt, char *str, int k, int *flag)
{
	int i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == 34 || str[i] == 39)
			i = quotes_skip(str, i);

		if (str[i] == '<' && str[i + 1] == '<')
		{
			*flag = -1;
			str = redirect_skip(&ipt->arg[k]->heradock, str, &i, flag);
		}
		else if (str[i] == '<')
			str = redirect_skip(&ipt->arg[k]->infile, str, &i, flag);
		else if (str[i] == '>' && str[i + 1] == '>')
		{
			*flag = 1;
			str = redirect_skip(&ipt->arg[k]->append_outfile, str, &i, flag);
		}
		else if (str[i] == '>')
		{
			*flag = 1;
			str = redirect_skip(&ipt->arg[k]->outfile, str, &i, flag);
		}
		if (!str)
			return (NULL);
		if (*flag != 0)
		{
			// printf("girdi flag=%d\n",*flag);
			return (NULL);
		}
	}
	return (str);
}

int	arg_parse2(t_input *ipt, int i, int j, int k)
{
	char *fakestr;
	char *temp;
	int flag;

	flag = 0;
	if (ipt->input[i + 1] == '\0')
		i++;
	fakestr = ft_substr(ipt->input, j, i - j);
	ipt->arg[k] = ft_calloc(1, sizeof(t_pro));
	if (!ipt->arg[k])
		return (0);
	temp = fakestr;
	fakestr = redirect_convert(ipt, fakestr, k, &flag);
	// if (temp !=fakestr)
	// 	free(temp);
	// printf("flag=%d\n",flag);
	if (flag == 0)
	{
		arg_convert(ipt, fakestr, k);
	}
	free(fakestr);
	return (i);
}

void	arg_parse(t_input *ipt, int len, int k)
{
	int i;
	int j;
	int flag;

	flag = 0;
	i = -1;
	j = 0;
	ipt->arg = malloc(sizeof(t_pro *) * (ipt->pipe + 2)); // burası +1di
	if (!ipt->arg)
		return ;
	while (ipt->input[++i] && i < len)
	// len'i koymayınca fazladan 1 kere daha giriyor
	{
		flag++;
		if (ipt->input[i] == 34 || ipt->input[i] == 39)
		{
			i = quotes_skip(ipt->input, i);
			i--;
		}
		if (ipt->input[i] == '\0' || ipt->input[i] == '|' || (ipt->input[i
				+ 1] == '\0'))
		{
			i = arg_parse2(ipt, i, j, k);
			k++;
			j = i + 1;
		}
	}
	if (flag == 0)
		ipt->arg[ipt->pipe] = NULL;
	else
		ipt->arg[ipt->pipe + 1] = NULL;
}