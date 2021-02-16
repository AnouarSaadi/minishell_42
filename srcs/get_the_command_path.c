/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_the_command_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 17:06:58 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/16 18:15:03 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int check_the_path_command(char *pathname)
{
	struct stat stat_buf;

	if (stat(pathname, &stat_buf) != 0)
	{
		return (0);
	}
	return (1);
}

static int is_directory(char *path)
{
	DIR *dir;

	if (!(dir = opendir(path)))
		return (0);
	closedir(dir);
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

int concat_cwd_cmd(char **cmd, t_exec *exec)
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
		exec->status = 1;
		return (0);
	}
	ft_free_arr((void **)&bin);
	return (1);
}

int chech_the_path(char **envp, char *args)
{
	char **sp;
	char *path;
	int check;
	int i;

	check = 0;
	path = get_var_env(envp, "PATH");
	sp = ft_split(path, ':');
	i = 0;
	while (sp[i])
	{
		if (ft_strncmp(args, sp[i], ft_strlen(sp[i])) == 0)
		{
			ft_free_2dem_arr((void ***)&sp);
			return (1);
		}
		i++;
	}
	ft_free_2dem_arr((void ***)&sp);
	return (0);
}

int check_exec_dot(t_exec *exec)
{
	char *bin;

	if (exec->args[0][0] == '.' && exec->args[0][1] == '/')
	{
		bin = concat_path_name(get_work_dir_path(), exec->args);
		if (ft_strcmp(bin, get_var_env(exec->envp, "_")) == 0)
		{
			exec->args[0] = ft_strdup(bin);
			ft_free_arr((void **)&bin);
			return (1);
		}
		else if (is_directory(bin))
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(exec->args[0], 2);
			ft_putendl_fd(": is a directory", 2);
			ft_free_2dem_arr((void ***)&(exec->args));
			ft_free_arr((void **)&bin);
			exec->status = 126;
			return (2);
		}
		else
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(exec->args[0], 2);
			ft_putendl_fd(": No such file or directory", 2);
			ft_free_2dem_arr((void ***)&(exec->args));
			ft_free_arr((void **)&bin);
			exec->status = 1;
			return (2);
		}
	}
	else if (exec->args[0][0] == '.' && ft_strlen(exec->args[0]) == 1)
	{
		ft_free_2dem_arr((void ***)&(exec->args));
		ft_putendl_fd("bash: .: filename argument required", 2);
		ft_putendl_fd(".: usage: . filename [arguments]", 2);
		exec->status = 2;
		return (2);
	}
	return (0);
}

int get_cmd_path(t_exec *exec)
{
	char *path;
	char **path_sp;
	char *bin;
	int i;

	if ((i = check_exec_dot(exec)) == 0)
	{
		if (!chech_the_path(exec->envp, exec->args[0]))
		{
			path = get_var_env(exec->envp, "PATH");
			if (ft_strcmp(path, ""))
			{
				path_sp = ft_split(path, ':');
				i = 0;
				while (path_sp[i])
				{
					bin = concat_path_name(path_sp[i], &(exec->args[0]));
					if (check_the_path_command(bin) == 1)
						break;
					i++;
				}
				ft_free_2dem_arr((void ***)&path_sp);
				if (is_directory(bin))
				{
					ft_putstr_fd("bash: ", 2);
					ft_putstr_fd(exec->args[0], 2);
					ft_putendl_fd(": is a directory", 2);
					ft_free_arr((void **)&bin);
					exec->status = 126;
					return (0);
				}
				if (!check_the_path_command(bin))
				{
					ft_putstr_fd("bash: ", 2);
					ft_putstr_fd(exec->args[0], 2);
					ft_putendl_fd(": command not found", 2);
					ft_free_arr((void **)&bin);
					exec->status = 127;
					return (0);
				}
				exec->args[0] = ft_strdup(bin);
				ft_free_arr((void **)&bin);
			}
			else
				return (concat_cwd_cmd(&(exec->args[0]), exec));
			ft_free_arr((void **)&path);
		}
		return (1);
	}
	else if (i == 2)
		return (0);
	return (1);
}