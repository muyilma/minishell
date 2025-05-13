#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void ft_exit(char **args)
{
    int exit_status;
    int i;

    printf("exit\n");
    if (!args || !args[0])
    {
        exit(0);
    }
    exit_status = ft_atoi(args[0]);
    if (args[1])
    {
        printf("exit: too many arguments\n");
        return;
    }
    i = 0;
    while (args[0][i])
    {
        if (!ft_isdigit(args[0][i]) && !(i == 0 && (args[0][i] == '-' || args[0][i] == '+')))
        {
            printf("exit: %s: numeric argument required\n", args[0]);
            exit(255);
        }
        i++;
    }
    exit(exit_status & 0xFF);
}
