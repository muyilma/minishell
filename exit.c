#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int ft_exit(char **args)
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
        ft_print_error(NULL,"exit: too many arguments", NULL, 1);
        return(1);
    }
    i = 0;
    while (args[0][i])
    {
        if (!ft_isdigit(args[0][i]) && !(i == 0 && (args[0][i] == '-' || args[0][i] == '+')))
        {
            ft_print_error("bash: exit:",": numeric argument required", args, 2);
            exit(2);
        }
        i++;
    }
    exit(exit_status & 0xFF);
}
