/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omgorege <omgorege@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:38:39 by musyilma          #+#    #+#             */
/*   Updated: 2025/05/27 15:16:27 by omgorege         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

int	ft_strcmp(char *s1, char *s2)
{
	unsigned int	i;

	i = 0;
	while (s1[i] == s2[i] && (s1[i] != 0 || s1[i] != 0))
	{
		i++;
	}
	return (s1[i] - s2[i]);
}

int	redirect_heredoc_write(int *fd, char *delimiter, int heredoc_status)
{
	char	*input;

	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			write(2, "minishell: warning: here-document at line by", 45);
			ft_print_error("end-of-file (wanted `", "')", &delimiter, 2);
			heredoc_status = -1;
			break ;
		}
		if (ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			break ;
		}
		write(*fd, input, ft_strlen(input));
		write(*fd, "\n", 1);
		free(input);
	}
	return (heredoc_status);
}

char	*cpy_heredock(char *str, t_shell *pro)
{
	static char	delimiter[10000];
	int			i;

	i = -1;
	while (str[++i])
		delimiter[i] = str[i];
	delimiter[i] = '\0';
	ft_executer_free(pro);
	ft_free(pro->env);
	free(pro);
	return (delimiter);
}

void	redirect_heredoc_to_stdin(char *delimiter, int built_in, t_shell *pro)
{
	int		fd[2];
	pid_t	pid;
	char	*str;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		str = cpy_heredock(delimiter, pro);
			g_signal_exit = 1;
		close(fd[0]);
		if (redirect_heredoc_write(&fd[1], str, 0) == -1)
			exit(0);
		close(fd[1]);
		exit(0);
	}
	else
	{
		close(fd[1]);
		if (wait_child(pid) == 130)
			return ;
		if (built_in == 0)
			dup2(fd[0], 0);
	}
}
