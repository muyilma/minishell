/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 13:23:31 by musyilma          #+#    #+#             */
/*   Updated: 2024/10/15 14:43:07 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*a;
	char	*b;
	size_t	i;

	b = (char *)src;
	a = dst;
	i = 0;
	if (a == NULL && b == NULL)
	{
		return (NULL);
	}
	while (n > 0)
	{
		n--;
		*(a + i) = *(b + i);
		i++;
	}
	return (dst);
}
