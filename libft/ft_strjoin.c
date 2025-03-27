/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musyilma <musyilma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 14:16:07 by musyilma          #+#    #+#             */
/*   Updated: 2024/11/02 09:01:21 by musyilma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		a;
	char	*str;
	int		i;
	int		b;

	a = ft_strlen(s1) + ft_strlen(s2);
	str = malloc(a + 1);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	b = 0;
	while (s2[b])
	{
		str[i] = s2[b];
		b++;
		i++;
	}
	str[i] = '\0';
	return (str);
}
