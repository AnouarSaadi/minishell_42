/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 11:27:37 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/07 11:59:21 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	char			*res;
	unsigned int	i;
	size_t			len;

	str = (char *)s;
	if (!str || !f)
		return (NULL);
	i = 0;
	len = ft_strlen(str);
	if (!(res = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (str[i])
	{
		res[i] = (*f)(i, str[i]);
		i++;
	}
	res[i] = '\0';
	return (res);
}
