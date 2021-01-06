/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 16:34:10 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/06 18:08:34 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd_function(void)
{
	char _pwd[PATH_MAX];

	if ((getcwd(_pwd, sizeof(_pwd)) == NULL))
		ft_putendl_fd(strerror(errno), 1);
	else
		ft_putendl_fd(_pwd, 1);
}

void		change_directory(char *_path)
{
	if (chdir(_path) == -1)
			ft_putendl_fd(strerror(errno), 1);
}

int 	count_vars_env(char **env_list)
{
	int count;

	count = 0;
	if (!env_list)
		return (0);
	while (env_list[count])
		count++;
	return (count);
}

static void	ft_free_split(char **split)
{
	int i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	**unset_function(char **envp, char *var_to_remove)
{
	char **new_envp;
	char **equ_split;
	int	len;
	int i;
	int j;

	len = count_vars_env(envp);
	i = 0;
	j = 0;
	if (!(new_envp = (char **)malloc(sizeof(char*) * len - 1)))
		return (NULL);
	while (envp[i])
	{
		equ_split = ft_split(envp[i], '=');
		if (strncmp(equ_split[0], var_to_remove, ft_strlen(equ_split[0])) != 0)
			new_envp[j++] = envp[i];
		i++;
		ft_free_split(equ_split);
	}
	new_envp[j] = NULL;
	return (new_envp);
}

void	env_function(char **envp)
{
	int i;

	i = 0;
	while (envp[i])
	{
		ft_putendl_fd(envp[i], 1);
		i++;
	}
}

char	**export_function(char **envp, char *var_to_add)
{
	char **new_envp;
	int len;
	int i;
	int j;

	len  = count_vars_env(envp);
	if (!(new_envp = (char **)malloc(sizeof(char*) * len + 1)))
		return (NULL);
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (strncmp(envp[i], "_", 1) == 0)
		{
			new_envp[j++] = var_to_add;
			new_envp[j++] = envp[i];

		}
		else
			new_envp[j++] = envp[i];
		i++;
	}
	new_envp[j] = NULL;
	return (new_envp);
}

void	echo_function(char *arg_to_print,int fd ,int newline)
{
	if (newline == 0)
		ft_putendl_fd(arg_to_print, fd);
	else
		ft_putstr_fd(arg_to_print, fd);
}

void	exit_function(int _id)
{
	exit(_id);
}