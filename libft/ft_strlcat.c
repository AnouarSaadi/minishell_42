/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 17:58:43 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/07 19:42:51 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	full;
	size_t	ldes;
	size_t	lsrc;
	int		i;

	lsrc = ft_strlen(src);
	if (!dest && !size)
		return (lsrc);
	i = 0;
	ldes = ft_strlen(dest);
	if (size <= ldes)
		full = size + lsrc;
	else
		full = ldes + lsrc;
	while (ldes + 1 < size && src[i])
	{
		dest[ldes] = src[i];
		ldes++;
		i++;
	}
	if (ldes < size)
		dest[ldes] = '\0';
	return (full);
}
