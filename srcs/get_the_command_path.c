/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_the_command_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 17:06:58 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/03 12:41:49 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int check_the_path_command(char *pathname)
{
	struct stat stat_buf;

	if (stat(pathname, &stat_buf) != 0)
		return (0);
	return (1);
}

static char *get_work_dir_path(void)
{
	return (getcwd(NULL, PATH_MAX));
}

char *concat_path_name(char *pathname, char **cmd)
{
	char *bin;

	if (!(bin = (char *)malloc(sizeof(char) * (ft_strlen(pathname) + ft_strlen(*cmd) + 1))))
		ft_putendl_fd("Error: Allocation failed!", 2); //check leak
	ft_strlcat(bin, pathname, ft_strlen(pathname) + 1);
	ft_strlcat(bin, "/", ft_strlen(bin) + 2);
	ft_strlcat(bin, *cmd, ft_strlen(bin) + ft_strlen(*cmd) + 1);
	return (bin);
}

int concat_cwd_cmd(char **cmd)
{
	char *cwd;
	char *bin;

	cwd = get_work_dir_path();
	bin = concat_path_name(cwd, cmd);
	if (check_the_path_command(bin) == 1)
		*cmd = ft_strdup(bin);
	else
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(*cmd, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (0);
	}
	ft_free_arr(bin);
	return (1);
}

int get_cmd_path(char **args, char **envp)
{
	char *path;
	char **path_split;
	char *bin;
	int i;

	i = 0;
	if (ft_strchr(args[0], '/') == NULL)
	{
		path = get_var_env(envp, "PATH");
		if (ft_strcmp(path, ""))
		{
			path_split = ft_split(path, ':');
			while (path_split[i])
			{
				bin = concat_path_name(path_split[i], &args[0]);
				if (check_the_path_command(bin) == 1)
					break;
				i++;
			}
			ft_free_2dem_arr(path_split);
			if (!check_the_path_command(bin))
			{
				ft_putstr_fd("bash: ", 2);
				ft_putstr_fd(args[0], 2);
				ft_putendl_fd(": command not found", 2);
				ft_free_arr(bin);
				return (0);
			}
			args[0] = ft_strdup(bin);
			ft_free_arr(bin);
		}
		else
			return (concat_cwd_cmd(&args[0]));
		ft_free_arr(path);
	}
	return (1);
}