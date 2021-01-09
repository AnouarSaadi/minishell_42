/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/09 12:32:10 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/09 12:49:34 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		edit_in_env(char **envp, char *cwd, char *old_pwd)
{
	int i;

	i = 0;
	while (envp[i])
	{
		if (!(ft_strncmp(envp[i], "PWD=", ft_strlen("PWD="))))
			envp[i] = ft_strdup(cwd);
		if (!(ft_strncmp(envp[i], "OLDPWD=", ft_strlen("OLDPWD="))))
			envp[i] = ft_strdup(old_pwd);
		i++;
	}
}

char *get_working_directory(void)
{
	char *_cwd;
	char *cwd;

	if (!(_cwd = (char *)ft_calloc(sizeof(char), ft_strlen("PWD=") + PATH_MAX + 1)))
		return (NULL);
	ft_strlcat(_cwd, "PWD=", ft_strlen("PWD=") + 1);
	if (!(cwd = (char *)ft_calloc(sizeof(char), PATH_MAX + 1)))
		return (NULL);
	if (getcwd(cwd, PATH_MAX) == NULL)
		return (NULL);
	ft_strlcat(_cwd, cwd, ft_strlen(_cwd) + ft_strlen(cwd) + 1);
	return (_cwd);
}

void change_directory(char *_path, char **envp)
{
	int home;
	char *pwd;
	char *oldpwd;

	home = 0;
	if (_path == NULL)
	{
		_path = get_var_env(envp, "HOME");
		home = 1;
	}
	if (chdir(_path) == 0)
	{
		pwd = get_var_env(envp, "PWD");
		if (!(oldpwd = (char *)malloc(sizeof(char) * (ft_strlen("OLDPWD=") + ft_strlen(pwd) + 1))))
		{
			ft_putendl_fd("Error: Malloc failed!", 1);
			exit(1);
		}
		ft_strlcat(oldpwd, "OLDPWD=", ft_strlen("OLDPWD=") + 1);
		ft_strlcat(oldpwd, pwd, ft_strlen(oldpwd) + ft_strlen(pwd) + 1);
		pwd = get_working_directory();
		edit_in_env(envp, pwd, oldpwd);
		free(pwd);
		free(oldpwd);
		if (home == 1)
			free(_path);
	}
	else
		ft_putendl_fd(strerror(errno), 1);
}