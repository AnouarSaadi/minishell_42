/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 15:32:35 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/07 18:59:10 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*str;
	char	r;
	int		i;

	r = (char)c;
	str = (char *)s;
	i = 0;
	while (str[i])
	{
		if (str[i] == r)
			return (str + i);
		i++;
	}
	if (r == '\0')
		return (str + i);
	return (0);
}
