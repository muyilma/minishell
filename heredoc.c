#include "libft/libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int redirect_heredoc_write(int *fd ,char *delimiter, int  heredoc_status)
{
    char    *input;

    while (1)
    {
        input = readline("> ");
        if (!input)
        {
            heredoc_status = -1;
            break;
        }
        if (ft_strncmp(input, delimiter,ft_strlen(delimiter)) == 0)
        {
            free(input);
            break;
        }
        write(*fd, input, ft_strlen(input));
        write(*fd, "\n", 1);
        free(input);
    }
    return (heredoc_status);
}
int redirect_heredoc_to_stdin(char *delimiter)
{
    int     fd[2];
    int     heredoc_status;

    heredoc_status = 0;
    if (pipe(fd) == -1)
    {
        perror("minishell: pipe");
        return (-1);
    }
    redirect_heredoc_write(&fd[1],delimiter, heredoc_status);
    close(fd[1]);
    if (heredoc_status == -1)
    {
        close(fd[0]);
        return (-1);
    }
    dup2(fd[0], 0);
    close(fd[0]);
    
    return (0);
}

