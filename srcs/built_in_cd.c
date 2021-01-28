/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/09 12:32:10 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/28 15:42:58 by asaadi           ###   ########.fr       */
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

char		*get_working_directory(void)
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

void		change_directory(char *_path, char **envp, int *i)
{
	int		home;
	char	*pwd;
	char	*oldpwd;
	char	*aftertelda;

	home = 0;
	*i = 1;
	if (_path == NULL)
	{
		_path = get_var_env(envp, "HOME");
		home = 1;
	}
	else if (_path[0] == '~')
	{
		aftertelda = ft_strchr(_path, '~') + 1;
		_path = get_var_env(envp, "HOME");
		ft_strlcat(_path, aftertelda, ft_strlen(_path) + ft_strlen(aftertelda) + 1);
		home = 1;
	}
	if (chdir(_path) == 0)
	{
		pwd = get_var_env(envp, "PWD");
		oldpwd = ft_strjoin("OLDPWD=", pwd);
		pwd = get_working_directory();
		edit_in_env(envp, pwd, oldpwd);
		free(pwd);
		free(oldpwd);
		if (home == 1)
			free(_path);
	}
	else
	{
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(_path, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
	}
}