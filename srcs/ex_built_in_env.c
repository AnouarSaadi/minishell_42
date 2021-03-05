/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_built_in_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 19:04:06 by asaadi            #+#    #+#             */
/*   Updated: 2021/03/05 17:55:40 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		env_function(char **envp)
{
	int i;

	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
			ft_putendl_fd(envp[i], 1);
		i++;
	}
	return (0);
}

/*
** char **envp_cpy(char **env).
** function create a copy of envp.
** end use that copy in all the program.
*/

char	**envp_cpy(char **env)
{
	char	**envp;
	int		i;

	if (!(envp = (char **)malloc(sizeof(char *) * (count_vars_env(env) + 1))))
		return (NULL);
	i = 0;
	while (env[i])
	{
		envp[i] = ft_strdup(env[i]);
		i++;
	}
	envp[i] = NULL;
	return (envp);
}
