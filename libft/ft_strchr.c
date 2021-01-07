/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 15:32:35 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/07 14:34:17 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*str;
	char	r;
	int		i;

	if (!s)
		return(NULL);
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
