/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 17:24:47 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/08 16:51:35 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*str;
	char	*fin;
	size_t	i;
	size_t	j;

	str = (char *)haystack;
	fin = (char *)needle;
	if (len == 0 || !*fin)
		return (str);
	i = 0;
	while (str[i] && i <= len)
	{
		j = 0;
		while (fin[j] && (j + i) < len)
		{
			if (str[i + j] != fin[j])
				break ;
			j++;
		}
		if (fin[j] == '\0')
			return (str + i);
		i++;
	}
	return (NULL);
}
