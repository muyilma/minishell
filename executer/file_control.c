/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_control.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:38:22 by musyilma          #+#    #+#             */
/*   Updated: 2025/05/27 18:03:00 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int	file_exists(char *path)
{
	struct stat	buffer;
	int			result;

	result = stat(path, &buffer);
	if (result == 0)
		return (1);
	return (0);
}

static int	is_directory(char *path)
{
	struct stat	path_stat;
	int			result;

	result = stat(path, &path_stat);
	if (result != 0)
		return (0);
	if (S_ISDIR(path_stat.st_mode))
		return (1);
	return (0);
}

int	check_access(char *path, int mode)
{
	int	result;

	result = access(path, mode);
	if (result == 0)
		return (1);
	return (0);
}

int	check_directory_access(char *path, char **error_msg)
{
	if (file_exists(path) == 0)
	{
		*error_msg = ": No such file or directory";
		return (0);
	}
	if (is_directory(path) == 0)
	{
		*error_msg = ": Not a directory";
		return (0);
	}
	if (check_access(path, R_OK) == 0 || check_access(path, X_OK) == 0)
	{
		*error_msg = ": Permission denied";
		return (0);
	}
	return (1);
}
