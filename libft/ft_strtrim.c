/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 14:34:36 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/08 10:46:37 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char		*ft_strtrim_from_start(char *s1, char const *s2)
{
	int	j;

	j = 0;
	while (s2[j])
	{
		if (*s1 == s2[j])
		{
			s1++;
			j = 0;
		}
		else
			j++;
	}
	return (s1);
}

static size_t	ft_strtrim_from_end(char *s1, char const *s2)
{
	size_t j;
	size_t len;

	j = 0;
	len = ft_strlen(s1) - 1;
	while (s2[j])
	{
		if (s1[len] == s2[j])
		{
			len--;
			j = 0;
		}
		else
			j++;
	}
	return (len + 1);
}

char			*ft_strtrim(char const *s1, char const *set)
{
	char	*trim;
	char	*strim;
	size_t	len;
	size_t	i;
	char	*from_str;

	strim = (char *)s1;
	if (!strim)
		return (NULL);
	i = 0;
	len = 0;
	from_str = ft_strtrim_from_start(strim, set);
	if (*from_str)
		len = ft_strtrim_from_end(from_str, set);
	if (!(trim = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (i < len)
	{
		trim[i] = from_str[i];
		i++;
	}
	trim[i] = '\0';
	return (trim);
}
