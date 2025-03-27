#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct s_pro
{
	char			*str;
	int				op;
	int				pipe;
	int				arg;
	int				command;
}					t_pro;

typedef struct s_arg
{
	t_pro			**arg;
	struct s_arg	*next;
}					t_arg;

int	main(void)
{
	t_arg	*hola;
	t_arg	*temp;
	char	*str;
    int		i;
	int		j;
	int		k;
	int		l;

	str = "< infile  grep m | < hola grep n | cat > adana.txt";
	i = 0;
	j = 0;
	k = 0;
	l = 0;
	while (i < 3)
	{
		if (i == 0)
		{
			hola = malloc(sizeof(t_arg));
			hola->next = NULL;
			temp = hola;
            i++;
			continue ;
		}
		temp->next = malloc(sizeof(t_arg));
		temp->next->next = NULL;
		temp = temp->next;
        i++;
	}

    temp=hola;
    i=0;

    while (temp!=NULL)
    {
        temp->arg=malloc(sizeof(t_pro));
        while (str[i]!='|' && str[i]!='\0')
        {
            if ((str[i]==' ' && str[i+1]!=' ') || str[i+1]=='|' || str[i+1]=='\0')
            {
                temp->arg[j]=malloc(sizeof(t_pro));
                temp->arg[j]->str=malloc(i-k);
                while (k<=i)
                {
                    temp->arg[j]->str[l]=str[k];
                    k++;
                    l++;
                }
                temp->arg[j]->str[l]='\0';
                l=0;
                j++;
            }
            i++;
            
        }
        i++;
        j=0;
        temp=temp->next;
    }
    
    // temp=hola;
    // i=0;
    // while (temp!=NULL)
    // {
    //     while (temp->arg[i])
    //     {
    //         printf("%s\n",temp->arg[i]->str);
    //         i++;
    //     }
    //     i=0;
    //     temp=temp->next;
    // }

}
