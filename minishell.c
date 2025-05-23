#include "libft/libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1

int		g_signal_exit = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	if (g_signal_exit == 1){
		write(1, "\n", 1);
		g_signal_exit=23;
		return;
	}
	else if (g_signal_exit == 2)
	{
		write(1, "\n", 1);
		exit(130);
	}
	else if (g_signal_exit == 0)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

char **copy_env(char **env, int b)
{
	int i = 0;
	char **copy;

	while (env[i])
		i++;
	copy = malloc(sizeof(char *) * (i + b + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (env[i])
	{
		copy[i] = ft_strdup(env[i]);
		if (!copy[i])
		{
			while (i--)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}


char	*ft_read(int fd, char *str)
{
	char *readstr;
	int byte;
	char *tmp;

	readstr = ft_calloc(BUFFER_SIZE + 1, 1);
	byte = 1;
	if (!str)
		str = ft_strdup("");
	while (ft_strchr(readstr, '\n') == 0 && byte > 0)
	{
		byte = read(fd, readstr, BUFFER_SIZE);
		if ((byte == -1) || (byte == 0 && !str[0]))
		{
			free(str);
			free(readstr);
			return (NULL);
		}
		readstr[byte] = '\0';
		tmp = str;
		str = ft_strjoin(str, readstr);
		free(tmp);
	}
	free(readstr);
	return (str);
}

char	*get_next_line(int fd)
{
	static char *afterstr;
	int j;
	char *beforestr;
	char *temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	beforestr = NULL;
	j = -1;
	afterstr = ft_read(fd, afterstr);
	while (afterstr && afterstr[++j])
	{
		if (afterstr[j] == '\n' || afterstr[j + 1] == '\0')
		{
			j++;
			beforestr = ft_substr(afterstr, 0, j);
			temp = afterstr;
			afterstr = ft_strdup(afterstr + j);
			free(temp);
			return (beforestr);
		}
	}
	return (beforestr);
}

void	read_line(t_shell *input, char **env, int code)
{
	input->input = NULL;
	input->isprint = 0;
	input->isalpha = 0;
	input->pipe = 0;
	input->error = 0;
	input->operator= 0;
	// input->dot = 0;
	input->after_str = 0;
	input->quotes = 0;
	input->dollar = 0;
	input->env = env;
	input->arg = NULL;
	input->exit_code = code;

	// input->input = readline("minishell:");
	// if (!input->input)
	// {
	// 	//ft_print_error(NULL, "exit", NULL, 1);
	// 	exit(0);
	// }
	if (isatty(fileno(stdin)))
		input->input = readline("minishell:");
	// if (!input->input)
	// {
	// 	exit(0);
	// }
	else
	{
		char *line;
		line = get_next_line(fileno(stdin));
		if (!line)
		{
			exit(0);
		}
		
		input->input = ft_strtrim(line, "\n");
		free(line);
	}
	add_history(input->input);
}


int	ft_executer(t_shell *input)
{
	int exit;
	free(input->input);
	exit = execute_pipe(input, 0);
	ft_executer_free(input);
	return (exit);
}

int	ft_error(t_shell *input)
{
	
	if (input->error == 2)
		write(2, "minishell: open quotes \"\'", 26);
	else if (input->error == 3)
		write(2, "minishell: syntax error near unexpected token `newline'\n",
			57);
	free(input->input);
	free(input);
	if (input->error==2 || input->error==3)
		exit (2);
	exit (0);
}

int	main(int ac, char **av, char **env)
{
	t_shell *input;
	int exit_code;
	char 	**new_env;

	(void)av;
	(void)ac;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT,SIG_IGN);
	exit_code = 0;
	new_env = copy_env(env,0);
	while (1)
	{
		input = malloc(sizeof(t_shell));
		read_line(input, new_env, exit_code);
		ft_parser(input);
		if (input->error == 0)
		{
			exit_code = ft_executer(input);
			if (input->env != new_env)
			{
				// ft_free(new_env);
				new_env = input->env;
			}
			free(input);
		}
		else
			exit_code=ft_error(input);
	}
}
