/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 11:04:58 by musyilma          #+#    #+#             */
/*   Updated: 2024/10/19 18:09:02 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	a;

	a = ft_strlen(s);
	if ((char)c == '\0')
		return ((char *)&s[a]);
	a -= 1;
	while (a >= 0)
	{
		if (s[a] == (char)c)
		{
			return ((char *)s + a);
		}
		a--;
	}
	return (NULL);
}
