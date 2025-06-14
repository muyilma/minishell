/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:38:31 by musyilma          #+#    #+#             */
/*   Updated: 2025/05/28 10:29:38 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <unistd.h>

static void	redirect_input(char *filename, t_shell *pro)
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

static void	redirect_output_append(char *filename, t_shell *pro)
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
		ft_print_error("minishell:", ": No such file or directory", &filename,
			2);
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

static void	redirect_output(char *filename, t_shell *pro)
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
		ft_print_error("minishell:", ": No such file or directory", &filename,
			2);
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

int	heredoc_control(t_cmd *pro, int in, t_shell *input)
{
	if (pro->heradock)
	{
		dup2(in, 0);
		redirect_heredoc_to_stdin(pro->heradock, 0, input);
		if (*get_exit_status_code() == 130)
			return (130);
		return (0);
	}
	return (1);
}

void	handle_redirections(t_shell *shell, t_cmd *pro)
{
	if (pro->infile)
		redirect_input(pro->infile, shell);
	if (pro->outfile)
		redirect_output(pro->outfile, shell);
	if (pro->append_outfile)
		redirect_output_append(pro->append_outfile, shell);
}
