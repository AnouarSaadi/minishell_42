/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 11:11:43 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/07 17:41:59 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*s;
	unsigned char	r;
	size_t			i;

	s = (unsigned char*)b;
	r = (unsigned char)c;
	i = 0;
	while (i < len)
		s[i++] = r;
	return (s);
}
