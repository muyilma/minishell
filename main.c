#include "libft/libft.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct s_pro
{
	char	*str;
	int		op;
	int		pipe;
	int		arg;
	int		command;
}			t_pro;

int	main(void)
{
	t_pro	**hola;
	char	*str;
	int		i;
	int		j;
	int		k;
	int		l;
	int		flag;
	char	qut;

	str = " < infile grep m | < hola grep n | cat > adana.txt";
	i = 0;
	k = 0;
	hola=malloc(sizeof(t_pro *));
	while (str[i])
	{
		while (str[i] == ' ')
		i++;
		j=i;
		flag=0;
		while (str[i] != ' ' && str[i] != '\0')
		{
			if (str[i] == 34 || str[i] == 39)
			{
				flag+=2;
				qut = str[i];
				i++;
				while (str[i] != qut)
				i++;
				i++;
			}
			i++;
		}
		//printf("----%d\n",k);
		l = 0;
		hola[k]=malloc(sizeof(t_pro));
		hola[k]->str=malloc(i-j-flag+1);
		while (j<i && str[i] != '\0')
		{
			if (str[j] == 34 || str[j] == 39)
			{
				j++;
				continue;
			}
			hola[k]->str[l]=str[j];
			l++;
			j++;
		}
		hola[k]->str[l]='\0';
		k++;
		
	}
	i=-1;
	k=-1;
	while (hola[++k])
		printf("%s\n", hola[k]->str);
}
