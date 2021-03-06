/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_wildcard_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 15:03:19 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/05 16:12:53 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

/*
** '*' match '.' if it's not the first character
*/

int		match(char *pattern, char *string, int p, int s)
{
	int ret;

	while ((size_t)p < ft_strlen(pattern))
	{
		if (pattern[p] == '*' && (string[s] != '.' || s != 0))
		{
			if ((size_t)s < ft_strlen(string)
					&& (ret = match(pattern, string, p, s + 1)))
				return (ret);
			else
				return (match(pattern, string, p + 1, s));
		}
		else if ((size_t)s < ft_strlen(string) && pattern[p] == string[s])
		{
			p++;
			s++;
		}
		else
			return (0);
	}
	if ((size_t)p == ft_strlen(pattern) && (size_t)s == ft_strlen(string))
		return (1);
	else
		return (0);
}

void	swipe(char **str1, char **str2)
{
	char *tmp;

	tmp = *str1;
	*str1 = *str2;
	*str2 = tmp;
}

void	free_dir_arr(char **dir_arr)
{
	int i;

	i = 0;
	while (dir_arr[i] != NULL)
	{
		free(dir_arr[i]);
		dir_arr[i] = NULL;
		i++;
	}
	free(dir_arr);
}

/*
** replace every duplicated char c in str to one e.i ***** -> *
*/

char	*change_to_one(char *str, char c)
{
	char	*res;
	int		i;
	int		j;

	res = (char *)malloc(sizeof(char) * ft_strlen(str) + 1);
	ft_bzero(res, ft_strlen(str) + 1);
	i = 0;
	j = 0;
	while ((size_t)i < ft_strlen(str))
	{
		res[j] = str[i];
		j++;
		if (str[i] == c)
			while (str[i] == c)
				i++;
		else
			i++;
	}
	return (res);
}
