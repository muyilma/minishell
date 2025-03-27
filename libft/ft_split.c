/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 08:50:01 by musyilma          #+#    #+#             */
/*   Updated: 2024/11/02 15:13:22 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static void	check(char **str, int len)
{
	while (len)
	{
		free(str[len]);
		len--;
	}
	free(str);
}

static void	spt(char **str, char const *s, char c)
{
	int	i;
	int	start;
	int	index;

	i = 0;
	start = 0;
	index = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[start] != c && s[start])
			start++;
		if (s[start] == c || start > i)
		{
			str[index] = ft_substr(s, i, start - i);
			if (str[index] == NULL)
				check(str, index - 1);
			i = start;
			index++;
		}
	}
	str[index] = NULL;
}

static int	word(char const *s, char c)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			len++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (len);
}

char	**ft_split(char const *s, char c)
{
	char	**a;
	int		len;

	len = word(s, c);
	a = malloc((len + 1) * sizeof(char *));
	if (!a)
		return (NULL);
	spt(a, s, c);
	return (a);
}
