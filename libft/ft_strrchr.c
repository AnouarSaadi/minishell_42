/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 16:01:21 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/07 18:59:54 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;
	char	r;
	int		len;

	str = (char *)s;
	r = (char)c;
	len = ft_strlen(s);
	while (len >= 0)
	{
		if (str[len] == r)
			return (str + len);
		len--;
	}
	return (NULL);
}
