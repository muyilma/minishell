#include "../libft/libft.h"
#include "../minishell.h"
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	ft_cd4(t_shell *pro, char *old_cwd, char *cwd)
{
	char	*new_pwd;
	char	*new_oldpwd;

	new_pwd = malloc(ft_strlen(cwd) + 5);
	if (!new_pwd)
		return (1);
	ft_strlcpy(new_pwd, "PWD=", 5);
	ft_strlcat(new_pwd, cwd, ft_strlen(cwd) + 5);
	pro->env = ft_setenv(pro->env, new_pwd);
	free(new_pwd);
	new_oldpwd = malloc(ft_strlen(old_cwd) + 8);
	if (!new_oldpwd)
		return (1);
	ft_strlcpy(new_oldpwd, "OLDPWD=", 8);
	ft_strlcat(new_oldpwd, old_cwd, ft_strlen(old_cwd) + 8);
	pro->env = ft_setenv(pro->env, new_oldpwd);
	free(new_oldpwd);
	return (0);
}

int	ft_cd3(t_shell *pro, char *old_cwd, char *cwd, char *path)
{
	char	*error_msg;

	if (!check_directory_access(path, &error_msg))
	{
		ft_print_error("minishell: cd:", error_msg, &path, 2);
		if (ft_strncmp(error_msg, ": Permission denied", 19) == 0)
			return (126);
		return (1);
	}
	if (chdir(path) != 0)
	{
		ft_print_error("minishell: cd:", ": Failed to change directory", &path,
			2);
		return (1);
	}
	if (getcwd(cwd, 1024) == NULL)
	{
		ft_print_error(NULL, "pwd not found", NULL, 1);
		return (1);
	}
	return (ft_cd4(pro, old_cwd, cwd));
}

int	ft_cd2(t_shell *pro, char **path)
{
	path[0] = ft_getenv(pro->env, "OLDPWD");
	if (!path[0])
	{
		ft_print_error(NULL, "minishell: cd: OLDPWD not set", NULL, 1);
		return (1);
	}
	printf("%s\n", path[0]);
	return (0);
}

int	ft_cd_home_control(char **args, t_shell *pro, char **path)
{
	*path = ft_getenv(pro->env, "HOME");
	if (!*path)
	{
		ft_print_error(NULL, "minishell: cd: HOME not set", args, 1);
		return (1);
	}
	return (0);
}

int	ft_cd(char **args, t_shell *pro)
{
	char	*path;
	int		exit;
	char	cwd[1024];
	char	old_cwd[1024];

	if (getcwd(old_cwd, sizeof(old_cwd)) == NULL)
	{
		ft_print_error(NULL, "pwd not found", NULL, 1);
		return (1);
	}
	if (!args || !args[0])
	{
		if (ft_cd_home_control(args, pro, &path) == 1)
			return (1);
	}
	else if (ft_strncmp(args[0], "-", 2) == 0)
	{
		exit = ft_cd2(pro, &path);
		if (exit == 1)
			return (exit);
	}
	else
		path = args[0];
	exit = ft_cd3(pro, old_cwd, cwd, path);
	return (exit);
}
