/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 14:34:52 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/07 18:18:20 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char			*d;
	unsigned const char		*s;
	unsigned char			r;
	size_t					i;

	d = (unsigned char *)dst;
	s = (unsigned const char *)src;
	r = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		if (s[i] == r)
			return (d + i + 1);
		i++;
	}
	return (0);
}
