/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirect_access.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:37:53 by musyilma          #+#    #+#             */
/*   Updated: 2025/05/28 12:57:08 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <sys/stat.h>
#include <unistd.h>

static char	*check_command_cmd_access(char *cmd)
{
	struct stat	cmd_stat;
	int			stat_result;

	if (file_exists(cmd) == 0)
		return (": No such file or directory");
	stat_result = stat(cmd, &cmd_stat);
	if (stat_result == 0)
	{
		if (S_ISDIR(cmd_stat.st_mode))
			return (": Is a directory");
		if (check_access(cmd, X_OK) == 0)
		{
			return (": Permission denied");
		}
		return (NULL);
	}
	return (NULL);
}

char	*check_command_access(char *cmd, char **env, char **error_msg)
{
	char	*cmd_path;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/') || (cmd[0] == '.'
			&& cmd[1] == '.' && cmd[2] == '/'))
	{
		*error_msg = check_command_cmd_access(cmd);
		if (*error_msg == NULL)
			return (ft_strdup(cmd));
		return (NULL);
	}
	else
	{
		cmd_path = pathc(cmd, env, -1);
		if (cmd_path == NULL)
		{
			*error_msg = ": command not found";
			return (NULL);
		}
		return (cmd_path);
	}
	*error_msg = ": Unknown error";
	return (NULL);
}

int	check_redirect_access_input(char *filename, char **error_msg)
{
	if (file_exists(filename) == 0)
	{
		*error_msg = ": No such file or directory";
		return (-1);
	}
	if (check_access(filename, R_OK) == 0)
	{
		*error_msg = ": Permission denied";
		return (-1);
	}
	return (0);
}

int	check_redirect_access(char *filename, char **error_msg)
{
	if (file_exists(filename) == 1 && check_access(filename, W_OK) == 0)
	{
		*error_msg = ": Permission denied";
		return (-1);
	}
	return (0);
}
