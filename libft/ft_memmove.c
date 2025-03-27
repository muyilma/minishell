/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:05:03 by musyilma          #+#    #+#             */
/*   Updated: 2024/10/18 15:20:48 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*a;
	char	*b;

	a = dst;
	b = (char *)src;
	if (a == NULL && b == NULL)
		return (NULL);
	if (a > b)
	{
		while (len)
		{
			*(a + (len - 1)) = *(b + (len - 1));
			len--;
		}
	}
	else if (b > a)
	{
		ft_memcpy(dst, src, len);
	}
	return (dst);
}
