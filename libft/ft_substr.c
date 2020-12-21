/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 11:53:11 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/02 12:46:45 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;
	size_t	j;
	size_t	slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start > slen)
		return (ft_strdup(""));
	if (len > slen - start)
		len = slen - start;
	if (!(sub = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = start;
	j = 0;
	if (i < len)
		while (j < len && s[i])
			sub[j++] = s[i++];
	sub[j] = '\0';
	return (sub);
}
