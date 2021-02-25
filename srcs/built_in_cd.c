/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/09 12:32:10 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/25 14:56:06 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		check_pwd__oldpwd(t_exec *exec, char *arg)
{
	int i;

	i = -1;
	while (exec->envp[++i])
		if (!ft_strncmp(exec->envp[i], arg, ft_strlen(arg)))
			return (1);
	return (0);
}

static void		edit_in_env(t_exec *exec, char *cwd, char *old_pwd)
{
	int i;

	i = 0;
	while (exec->envp[i])
	{
		if (!(ft_strncmp(exec->envp[i], "PWD=", ft_strlen("PWD="))))
			exec->envp[i] = ft_strdup(cwd);
		if (!(ft_strncmp(exec->envp[i], "OLDPWD=", ft_strlen("OLDPWD="))))
			exec->envp[i] = ft_strdup(old_pwd);
		i++;
	}
}

static char		*get_working_directory(void)
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

static int	ft_ch__dir(t_exec *exec, char **path, int home)
{
	char	*oldpwd;
	char	*pwd;

	if (chdir((*path)) == 0)
	{
		pwd = get_var_env(exec->envp, "PWD");
		oldpwd = ft_strjoin("OLDPWD=", pwd);
		pwd = get_working_directory();
		edit_in_env(exec, pwd, oldpwd);
		ft_free_arr((void**)&pwd);
		ft_free_arr((void**)&oldpwd);
		if (home)
			ft_free_arr((void**)&(*path));
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd((*path), 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (1);
	}
	return (0);
}

int		change_directory(char *path, t_exec *exec)
{
	int		home;
	char	*aftertelda;

	home = 0;
	if (!path)
	{
		path = get_var_env(exec->envp, "HOME");
		home++;
	}
	else if (path && !ft_strncmp(path, "~", 1))
	{
		aftertelda = ft_strchr(path, '~') + 1;
		path = get_var_env(exec->envp, "HOME");
		ft_strlcat(path, aftertelda, ft_strlen(path) + ft_strlen(aftertelda) + 1);
		home++;
	}
	return(ft_ch__dir(exec, &path, home));
}
