#include "libft/libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


void	ft_env(char **env, char **args, t_input *pro)
{
	int i;

	i = 0;
	if (args && args[0])
	{
		if (ft_strchr(args[0], '=') == NULL)
		{
			perror("env:");
			exit(0);
		}
		ft_export(&args[0], pro, 1);
	}
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
			printf("%s\n", env[i]);
		i++;
	}
}

void	built_in3(char **args, t_input *pro)
{
	if (ft_strncmp(args[0], "unset", 5) == 0)
	{
		ft_unset(&args[1], pro);
		exit(0);
	}
	if (ft_strncmp(args[0], "export", 7) == 0)
	{
		ft_export(&args[1], pro, 0);
		exit(0);
	}
	if (ft_strncmp(args[0], "pwd", 4) == 0)
	{
		ft_pwd();
		exit(0);
	}
}

void	built_in(char **args, t_input *pro)
{
	if (ft_strncmp(args[0], "echo", 5) == 0)
	{
		ft_echo(&args[1], 0, 1);
		exit(0);
	}
	if (ft_strncmp(args[0], "env", 4) == 0)
	{
		ft_env(pro->env, &args[1], pro);
		exit(0);
	}
	if (ft_strncmp(args[0], "exit", 5) == 0)
	{
		ft_exit(&args[1]);
		exit(0);
	}
	if (ft_strncmp(args[0], "cd", 3) == 0)
	{
		ft_cd(&args[1], pro);
		exit(0);
	}
	built_in3(args, pro);
}

int built_in2(char **args, t_input *pro, t_pro *arg)
{
    int original_stdout = -1;
    int original_stdin = -1;
    int result;

    if (!args || !args[0])
        return (2);

    // Eğer yönlendirme varsa, orijinal dosya tanımlayıcılarını kaydet
    if ((ft_strncmp(args[0], "export", 7) == 0 || 
         ft_strncmp(args[0], "exit", 5) == 0 || 
         ft_strncmp(args[0], "cd", 3) == 0 || 
         ft_strncmp(args[0], "unset", 5) == 0) && 
        (arg->infile || arg->outfile || arg->append_outfile))
    {
        if (arg->outfile || arg->append_outfile)
            original_stdout = dup(1);
        
        if (arg->infile)
            original_stdin = dup(0);
        
        handle_redirections(arg);
    }

    // Built-in komutları işle
    if (ft_strncmp(args[0], "exit", 5) == 0)
        result = ft_exit(&args[1]);
    else if (ft_strncmp(args[0], "cd", 3) == 0)
    {
        if (args[2])
        {
            ft_print_error(NULL, "minishell: cd: too many arguments", NULL, 1);
            result = 1;
        }
        else
            result = ft_cd(&args[1], pro);
    }
    else if (ft_strncmp(args[0], "unset", 5) == 0)
        result = ft_unset(&args[1], pro);
    else if (ft_strncmp(args[0], "export", 7) == 0)
        result = ft_export(&args[1], pro, 0);
    else
        result = 2;

    if (original_stdout != -1)
    {
        dup2(original_stdout, STDOUT_FILENO);
        close(original_stdout);
    }
    
    if (original_stdin != -1)
    {
        dup2(original_stdin, STDIN_FILENO);
        close(original_stdin);
    }

    return (result);
}
