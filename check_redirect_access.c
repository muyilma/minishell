#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
int check_redirect_access_input(char *filename, char **error_msg)
{
    if (file_exists(filename) == 0)
    {
        *error_msg = ": No such file or directory";
        return (-1);
    }
    if (check_access(filename, R_OK) == 0)
    {
        *error_msg = ": Permission denied";
        return (-1);
    }
    return (0);
}

int check_redirect_access(char *filename, char **error_msg)
{
    if (file_exists(filename) == 1 && check_access(filename, W_OK) == 0)
    {
        *error_msg = ": Permission denied";
        return (-1);
    }
    return (0);
}
