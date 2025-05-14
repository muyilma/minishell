#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	redirect_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
    
	if (fd == -1)
	{
		perror("minishell");
		exit(1);
	}
	dup2(fd, 0);
	close(fd);
}

void	redirect_output(char *filename, int append)
{
	int	fd;
    
	if (append)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		exit(1);
	}
	dup2(fd, 1);
	close(fd);
}

void	handle_redirections(t_pro *pro)
{      
	if (pro->infile)
		redirect_input(pro->infile);
    
	if (pro->outfile)
		redirect_output(pro->outfile, 0);
    
    if (pro->append_outfile)
        redirect_output(pro->append_outfile, 1);
}
