/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 12:19:40 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/08 10:12:32 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t len)
{
	unsigned char	*s;
	unsigned char	*d;

	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	if (!s && !d)
		return (0);
	if (d <= s)
		ft_memcpy(d, s, len);
	else
		while (len--)
			*(d + len) = *(s + len);
	return (dest);
}
