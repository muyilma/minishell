/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 14:38:59 by musyilma          #+#    #+#             */
/*   Updated: 2024/11/02 09:09:16 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;
	char	*b;

	i = 0;
	b = (char *)s;
	while (i < n)
	{
		if (b[i] == (char)c)
		{
			return (b + i);
		}
		i++;
	}
	return (NULL);
}
