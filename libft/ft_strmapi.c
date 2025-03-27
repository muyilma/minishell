/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 04:11:29 by musyilma          #+#    #+#             */
/*   Updated: 2024/11/02 09:02:36 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*a;
	int		i;

	i = 0;
	a = malloc(ft_strlen(s) + 1);
	if (a == NULL)
	{
		return (NULL);
	}
	while (s[i])
	{
		a[i] = (*f)(i, s[i]);
		i++;
	}
	a[i] = '\0';
	return (a);
}
