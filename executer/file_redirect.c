#include "../libft/libft.h"
#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void error_and_allocate(t_shell *pro, int exit_code)
{
	ft_executer_free(pro);
	exit(exit_code);
}

void	redirect_shell(char *filename, t_shell *pro)
{
	int		fd;
	char	*error_msg;

	if (check_redirect_access_input(filename, &error_msg) == -1)
	{
		ft_print_error("minishell:", error_msg, &filename, 2);
		error_and_allocate(pro, 1);
	}
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_print_error("minishell:", ": No such file or directory", &filename,
			2);
		error_and_allocate(pro, 1);
	}
	if (dup2(fd, 0) == -1)
	{
		close(fd);
		ft_print_error("minishell:", ": No such file or directory", &filename,
			2);
		error_and_allocate(pro, 1);
	}
}

void	redirect_output_append(char *filename, t_shell *pro)
{
	int		fd;
	char	*error_msg;

	if (check_redirect_access(filename, &error_msg) == -1)
	{
		ft_print_error("minishell:", error_msg, &filename, 2);
		error_and_allocate(pro, 1);
	}
	fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
	{
		ft_print_error("minishell:", ": No such file or directory", &filename, 2);
		error_and_allocate(pro, 1);
	}
	if (dup2(fd, 1) == -1)
	{
		close(fd);
		ft_print_error("minishell:", ": No such file or directory", &filename,
			2);
		error_and_allocate(pro, 1);
	}
}

void	redirect_output(char *filename, t_shell *pro)
{
	int		fd;
	char	*error_msg;

	if (check_redirect_access(filename, &error_msg) == -1)
	{
		ft_print_error("minishell:", error_msg, &filename, 2);
		error_and_allocate(pro, 1);
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_print_error("minishell:", ": No such file or directory", &filename, 2);
		error_and_allocate(pro, 1);
	}
	if (dup2(fd, 1) == -1)
	{
		close(fd);
		ft_print_error("minishell:", ": No such file or directory", &filename,
			2);
		error_and_allocate(pro, 1);
	}
}

int	heredoc_control(t_cmd *pro)
{
	if (pro->heradock)
	{
		if (redirect_heredoc_to_stdin(pro->heradock) == -1)
		{
			return (0);
		}
		return (0);
	}
	return (1);
}

void	handle_redirections(t_shell *shell,t_cmd *pro)
{
	if (pro->infile)
		redirect_shell(pro->infile,shell);
	if (pro->outfile)
		redirect_output(pro->outfile,shell);
	if (pro->append_outfile)
		redirect_output_append(pro->append_outfile,shell);
}
