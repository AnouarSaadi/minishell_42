/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 14:53:45 by asaadi            #+#    #+#             */
/*   Updated: 2020/12/03 14:17:41 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_alloc_t(const char *s, char c)
{
	int i;
	int count;
	int word;

	i = 0;
	count = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			if (word == 1)
			{
				word = 0;
				count++;
			}
		}
		else if (s[i] != c)
			word = 1;
		i++;
	}
	if (word)
		count++;
	return (count);
}

static int	ft_slen(int index, const char *str, char c)
{
	int i;

	i = index;
	while (str[i] != '\0' && str[i] != c)
		i++;
	return (i - index);
}

static char	**ft_failed_alloc(char **res, int j)
{
	j = j - 1;
	while (res[j])
	{
		free(res[j]);
		j--;
	}
	free(res);
	return (NULL);
}

static char	**ft_split_it(char **res, const char *s, char c)
{
	int i;
	int j;
	int k;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			i++;
		else
		{
			if (!(res[j] = malloc(sizeof(char) * (ft_slen(i, s, c) + 1))))
				return (ft_failed_alloc(res, j));
			k = 0;
			while (s[i] != c && s[i] != '\0')
				res[j][k++] = s[i++];
			res[j++][k] = '\0';
		}
	}
	res[j] = 0;
	return (res);
}

char		**ft_split(char const *s, char c)
{
	char	**res;
	int		word;

	if (!s)
		return (NULL);
	word = ft_count_alloc_t(s, c);
	if (!(res = malloc(sizeof(char *) * (word + 1))))
		return (NULL);
	res = ft_split_it(res, s, c);
	return (res);
}
