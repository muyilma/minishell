#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void redirect_input(char *filename)
{
    int fd;
    char *error_msg;
    
    if (check_redirect_access(filename, 0, &error_msg) == -1)
    {
        ft_print_error("minishell:", error_msg, &filename, 2);
        exit(1);
    }
    
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        ft_print_error("minishell:", ": Failed to open file", &filename, 2);
        exit(1);
    }
    
    if (dup2(fd, 0) == -1)
    {
        close(fd);
        ft_print_error("minishell:", ": Failed to redirect input", &filename, 2);
        exit(1);
    }
    
    close(fd);
}

void redirect_output_append(char *filename)
{
    int fd;
    char *error_msg;
    
    if (check_redirect_access(filename, 2, &error_msg) == -1)
    {
        ft_print_error("minishell:", error_msg, &filename, 2);
        exit(1);
    }
    
    fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        ft_print_error("minishell:", ": Failed to open file", &filename, 2);
        exit(1);
    }
    
    if (dup2(fd, 1) == -1)
    {
        close(fd);
        ft_print_error("minishell:", ": Failed to redirect output", &filename, 2);
        exit(1);
    }
    
    close(fd);
}

void redirect_output(char *filename)
{
    int fd;
    char *error_msg;
    
    if (check_redirect_access(filename, 1, &error_msg) == -1)
    {
        ft_print_error("minishell:", error_msg, &filename, 2);
        exit(1);
    }
    
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        ft_print_error("minishell:", ": Failed to open file", &filename, 2);
        exit(1);
    }
    
    if (dup2(fd, 1) == -1)
    {
        close(fd);
        ft_print_error("minishell:", ": Failed to redirect output", &filename, 2);
        exit(1);
    }
    
    close(fd);
}

void handle_redirections(t_pro *pro)
{
    if (pro->infile)
        redirect_input(pro->infile);
    
    if (pro->outfile)
        redirect_output(pro->outfile);
    
    if (pro->append_outfile)
        redirect_output_append(pro->append_outfile);
}