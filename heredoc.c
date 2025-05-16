#include "libft/libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int redirect_heredoc_to_stdin(char *delimiter)
{
    int     fd[2];
    char    *input;
    int     heredoc_status;

    heredoc_status = 0;
    if (pipe(fd) == -1)
    {
        perror("minishell: pipe");
        return (-1);
    }
    
    // Setup signal handling for heredoc
    // You may need to add signal handling functions
    
    while (1)
    {
        input = readline("> ");
        if (!input) // Handle EOF (Ctrl+D)
        {
            heredoc_status = -1;
            break;
        }
        
        if (strcmp(input, delimiter) == 0)
        {
            free(input);
            break;
        }
        
        // Write input line followed by a single newline
        write(fd[1], input, ft_strlen(input));
        write(fd[1], "\n", 1); // Write only 1 newline character
        free(input);
    }
    
    close(fd[1]); // Close write end
    
    if (heredoc_status == -1)
    {
        close(fd[0]); // Close read end if interrupted
        return (-1);
    }
    
    // Redirect stdin to read from pipe
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    
    return (0);
}

