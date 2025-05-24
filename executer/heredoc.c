#include "../libft/libft.h"
#include "../minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


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
		if (!input || g_signal_exit == 23)
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


void	redirect_heredoc_to_stdin(char *delimiter, int built_in)
{
	int		fd[2];
	pid_t	pid;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		close(fd[0]);
		if (redirect_heredoc_write(&fd[1], delimiter, 0) == -1)
			exit(1);
		close(fd[1]);
		exit(0);
	}
	else
	{
		close(fd[1]);
		if (wait_child(pid) == 130)
		{
			close(fd[0]);
			return;
		}
		if (built_in == 0)
		{
			dup2(fd[0], 0);
			close(fd[0]);
		}
	}
}

