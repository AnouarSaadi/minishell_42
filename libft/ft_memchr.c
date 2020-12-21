/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 14:25:39 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/08 10:20:22 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	r;
	size_t			i;

	str = (unsigned char *)s;
	r = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (str[i] == r)
			return (str + i);
		i++;
	}
	return (0);
}
