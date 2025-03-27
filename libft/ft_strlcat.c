/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 17:15:01 by musyilma          #+#    #+#             */
/*   Updated: 2024/11/02 08:37:52 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	a;
	size_t	b;

	a = ft_strlen(dst);
	b = a + ft_strlen(src);
	if (a >= dstsize)
		return (ft_strlen(src) + dstsize);
	dstsize -= a + 1;
	i = 0;
	while (dstsize > 0 && src[i])
	{
		dst[a] = src[i];
		i++;
		a++;
		dstsize--;
	}
	dst[a] = '\0';
	return (b);
}
