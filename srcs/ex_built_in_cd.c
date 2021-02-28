/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_built_in_cd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/09 12:32:10 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/27 19:14:25 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** static void	edit_in_env(t_exec *exec, char *var, char *arg)
** static void	edit_pwd__oldpwd(t_exec *exec, char *arg, int n)
** Two function working on editing PWD and OLDPWD in envp
** if we change the working directory
*/

static void		edit_in_env(t_exec *exec, char *var, char *arg)
{
	int i;

	i = -1;
	while (exec->envp[++i])
		if (!(ft_strncmp(exec->envp[i], var, ft_strlen(var))))
			exec->envp[i] = ft_strdup(arg);
}

static void		edit_pwd__oldpwd(t_exec *exec, char *arg, char *var)
{
	int i;
	int exist;

	i = -1;
	exist = 0;
	while (exec->envp[++i])
		if (!ft_strncmp(exec->envp[i], var, ft_strlen(var)))
			exist++;
	if (exist)
		edit_in_env(exec, var, arg);
	else
		export_func_2(exec, arg);
}

/*
** static char		*get_working_directory(void)
** function return a char* hold the new PWD.
*/

static char		*get_working_directory(void)
{
	char *cwd__;

	if (!(cwd__ = (char *)ft_calloc(sizeof(char),
		ft_strlen("PWD=") + PATH_MAX + 1)))
		return (NULL);
	ft_strlcat(cwd__, "PWD=", ft_strlen("PWD=") + 1);
	ft_strlcat(cwd__, getcwd(NULL, PATH_MAX), ft_strlen(cwd__) + PATH_MAX + 1);
	return (cwd__);
}

/*
** static int	ft_ch__dir(t_exec *exec, char **path, int home)
** function that do the all work in change directory by using 'chdir' function
*/

static int		ft_ch__dir(t_exec *exec, char **path, int home)
{
	char	*old__pwd;
	char	*pwd;

	if (chdir((*path)) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd((*path), 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (1);
	}
	pwd = get_var_env(exec->envp, "PWD");
	if (!(old__pwd = ft_strjoin("OLDPWD=", pwd)))
		return (ft_print__malloc(pwd, NULL, 1));
	if (!(pwd = get_working_directory()))
		return (ft_print__malloc(pwd, old__pwd, 1));
	edit_pwd__oldpwd(exec, old__pwd, "OLDPWD=");
	edit_pwd__oldpwd(exec, pwd, "PWD=");
	ft_free_arr((void**)&pwd);
	ft_free_arr((void**)&old__pwd);
	if (home)
		ft_free_arr((void**)&(*path));
	return (0);
}

int				change_directory(char *path, t_exec *exec)
{
	int		home;
	char	*a_telda;

	home = 0;
	if ((!path) || (path && !ft_strncmp(path, "~", 1)))
	{
		if (path && !ft_strncmp(path, "~", 1))
		{
			a_telda = ft_strchr(path, '~') + 1;
			path = get_var_env(exec->envp, "HOME");
			ft_strlcat(path, a_telda, ft_strlen(path) + ft_strlen(a_telda) + 1);
		}
		else
			path = get_var_env(exec->envp, "HOME");
		home++;
	}
	return (ft_ch__dir(exec, &path, home));
}
