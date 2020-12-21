/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 18:06:59 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/06 15:35:47 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strjoin(char const *s1, char const *s2)
{
	char	*pre;
	char	*suf;
	char	*res;
	int		i;
	int		j;

	pre = (char *)s1;
	suf = (char *)s2;
	if (!suf || !pre)
		return (NULL);
	res = (char *)malloc(sizeof(char) * (ft_strlen(pre) + ft_strlen(suf) + 1));
	if (!res)
		return (NULL);
	i = -1;
	j = 0;
	while (pre[++i])
		res[i] = pre[i];
	while (suf[j])
		res[i++] = suf[j++];
	res[i] = '\0';
	return (res);
}
