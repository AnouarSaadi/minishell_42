/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_the_command_path.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 10:49:11 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/07 14:37:24 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_the_path_command(char *pathname)
{
	struct stat stat_buf;

	if (stat(pathname, &stat_buf) != 0)
		return (0);
	return (1);
}

void find_the_cmd_path(char **args, char **envp)
{
	char	*path;
	char	**path_split;
	char	*bin;
	int		i;

	/* find the command path */
	i = 0;
	if (ft_strchr(args[0], '/') == NULL)
	{
		if ((path = check_var_env(envp, "PATH")) != NULL)
		{
			path_split = ft_split(path, ':');
			ft_free_arr(path);
			while (path_split[i])
			{
				if (!(bin = (char *)malloc(sizeof(char) * (ft_strlen(path_split[i]) + ft_strlen(args[0]) + 1))))
				{
					ft_putendl_fd("Allocation failed!", 1);
					exit(1);
				}
				ft_strlcat(bin, path_split[i], ft_strlen(bin) + ft_strlen(path_split[i]) + 1);
				ft_strlcat(bin, "/", ft_strlen(bin) + 2);
				ft_strlcat(bin, args[0], ft_strlen(bin) + ft_strlen(args[0]) + 1);
				if (check_the_path_command(bin) == 1)
					break;
				i++;
			}
			ft_free_2dem_arr(path_split);
			args[0] = ft_strdup(bin);
			ft_free_arr(bin);
		}
	}
}