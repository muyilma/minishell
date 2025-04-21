#include "libft/libft.h"
// #include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct s_pro
{
	char	**str;
	char	*outfile;
	char	*infile;
	char	*heradock;
	char	*append_outfile;
}			t_pro;

int	quadot_skip(char *str, int j)
{
	int		i;
	char	qut;

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

static int	word_count(char *str)
{
	int		i;
	int		count;
	char	qut;

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
				i = quadot_skip(str, i);
			else
				i++;
		}
	}
	return (count);
}

char *fonction(char *str,int start,int end,int size)
{
	int i;
	int j;
	char *newstr;

	newstr=malloc(size+1);
	i=-1;
	j=0;
	while (str[++i])
	{
		if (i==start)
		{
			while (i<=end)
				i++;
		}
		newstr[j]=str[i];
		j++;
	}
	newstr[j]='\0';
	return (newstr);
}

char	*redirect_convert(t_pro **stack, char *str,int k)
{
	int i;
	int j;
	int m;

	i=0;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
		i=quadot_skip(str,i);
		if (str[i]=='<')
		{
			j=i;
			i++;
			while (str[i]==' ')
			i++;
			m=i;
			while (str[i]!=' ')
			{
				if (str[i] == 34 || str[i] == 39)
				i=quadot_skip(str,i);
				i++;
			}
			stack[k]->infile=ft_substr(str,m,i-m);
			str=fonction(str,j,i,ft_strlen(str)-(i-j));
			i=-1;
		}

		i++;
	}
	return (str);
}

int	main(void)
{
	t_pro	**stack;
	char	*str;
	char	*fake_str;
	int		i;
	int		k;
	int		j;

	stack = malloc(sizeof(t_pro *) * 3);
	i = 0;
	k = 0;
	j = 0;
	str = "< infile grep m < in | < hola grep n | cat > adana.txt";
	while (str[i])
	{
		if (str[i] == '|' || str[i + 1] == '\0')
		{
			if (str[i + 1] == '\0')
			i++;
			fake_str = ft_substr(str, k, i - k);
			stack[j] = malloc(sizeof(t_pro));
			fake_str = redirect_convert(stack, fake_str,j);
			// executer_token(fake_str,&stack);
			printf("%s\n", fake_str);
			k = i + 1;
			j++;
			free(fake_str);
		}
		i++;
	}
}

char	*aaa(char **infile, char *str)
{
	*infile = ft_substr(str, 0, 5);
	str = ft_substr(str, 0, 1);
	return (str);
}

int	main(void)
{
	t_pro	**hola;
	char	*str;

	str = "musatafa";
	hola = malloc(sizeof(t_pro *));
	hola[0] = malloc(sizeof(t_pro));
	str = aaa(&hola[0]->infile, str);
	printf("%s\n", hola[0]->infile);
}
