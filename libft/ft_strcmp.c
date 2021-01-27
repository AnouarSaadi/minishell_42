/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/24 19:08:16 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/27 17:58:47 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strcmp(const char *s1, const char *s2)
{
	size_t i;

	i = 0;
	if ((!s1 || !s2))
		return (-1);
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (((s1[i] == '\0' && s2[i] != '\0') || (s2[i] == '\0' && s1[i] != '\0')))
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}
