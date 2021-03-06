/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_wildcard_dir_arr.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/04 15:53:26 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/05 16:56:42 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

size_t	dirlen(char *name)
{
	DIR				*d;
	struct dirent	*dir;
	size_t			len;

	len = 0;
	d = opendir(name);
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			len++;
		}
		closedir(d);
	}
	return (len);
}

void	sort_dir_arr(char **dir_arr)
{
	int		i;
	size_t	len;

	if (dir_arr[0] == NULL)
		return ;
	i = 1;
	while (dir_arr[i] != NULL)
	{
		len = (ft_strlen(dir_arr[i - 1]) > ft_strlen(dir_arr[i])) ?
			ft_strlen(dir_arr[i - 1]) : ft_strlen(dir_arr[i]);
		if (ft_strncmp(dir_arr[i - 1], dir_arr[i], len) > 0)
		{
			swipe(&dir_arr[i - 1], &dir_arr[i]);
			i = 0;
		}
		i++;
	}
}

int		is_dir(char *dir_name, char *sub_dir_name)
{
	char		*full_dir;
	char		*tmp;
	struct stat	statbuf;

	tmp = NULL;
	if (ft_strchr(dir_name, '/') == NULL)
	{
		tmp = ft_strjoin(dir_name, "/");
		full_dir = ft_strjoin(tmp, sub_dir_name);
	}
	else
		full_dir = ft_strjoin(dir_name, sub_dir_name);
	if (tmp != NULL)
		free(tmp);
	free(full_dir);
	if (stat(full_dir, &statbuf) == -1 || S_ISDIR(statbuf.st_mode) == 0)
		return (0);
	return (1);
}

/*
** onlydir is result of strchr of '/' on pattern NULL only dir must be returned
*/

char	**fill_dir_arr(DIR *d, char *onlydir, char *dir_name, char *pattern)
{
	struct dirent	*dir;
	size_t			i;
	char			**dir_arr;

	dir_arr = (char**)malloc(sizeof(char*) * (dirlen(dir_name) + 1));
	i = 0;
	while (d != NULL && (dir = readdir(d)) != NULL)
	{
		if ((onlydir == NULL || is_dir(dir_name, dir->d_name) == 1)
				&& match(pattern, dir->d_name, 0, 0) == 1)
		{
			dir_arr[i] = ft_strdup(dir->d_name);
			i++;
		}
	}
	dir_arr[i] = NULL;
	return (dir_arr);
}

char	**get_dir_arr(char *dir_name, char *onlydir, char *pattern)
{
	DIR		*d;
	char	**dir_arr;

	pattern = ft_strdup(pattern);
	if (dir_name == NULL)
		dir_name = ".";
	d = opendir(dir_name);
	if (onlydir != NULL)
		*(ft_strchr(pattern, '/')) = '\0';
	dir_arr = fill_dir_arr(d, onlydir, dir_name, pattern);
	if (d != NULL)
		closedir(d);
	free(pattern);
	return (dir_arr);
}
