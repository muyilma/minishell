#include "../libft/libft.h"
#include "../minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
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
		printf("---\n");
		if (!input || g_signal_exit==23)
		{
			printf("222222222\n");
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

int	redirect_heredoc_to_stdin(char *delimiter)
{
	int	fd[2];
	int	heredoc_status;

	g_signal_exit=1;
	heredoc_status = 0;
	if (pipe(fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	redirect_heredoc_write(&fd[1], delimiter, heredoc_status);
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
