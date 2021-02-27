/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_built_in_unset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 18:12:22 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/27 18:38:09 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 ** int 	count_vars_env(char **env_list)
 ** return number of vars in envp
*/

int			count_vars_env(char **env_list)
{
	int count__;

	count__ = 0;
	if (!env_list)
		return (0);
	while (env_list[count__])
		count__++;
	return (count__);
}

/*
 ** int   unset_function(t_exec *exec)
 ** unset var or multiple vars from envp
*/

static int	get_args_in_end(t_exec *exec, char *var)
{
	int		index[2];
	char	**equ;
	char	**env__;

	if (var)
	{
		if (!(env__ = (char **)malloc(sizeof(char *) *\
						count_vars_env(exec->envp))))
			return (ft_print__malloc(NULL, NULL, 1));
		index[0] = -1;
		index[1] = 0;
		while ((exec->envp)[++index[0]])
		{
			equ = ft_split((exec->envp)[index[0]], '=');
			if (ft_strcmp(equ[0], var))
				env__[index[1]++] = ft_strdup((exec->envp)[index[0]]);
			ft_free_2dem_arr((void***)&equ);
		}
		env__[index[1]] = NULL;
		ft_free_2dem_arr((void***)&(exec->envp));
		exec->envp = env__;
	}
	return (0);
}

int			unset_function(t_exec *exec)
{
	int index;
	int ret;

	index = 1;
	ret = 0;
	while (exec->args[index])
	{
		if ((ft_isalpha(exec->args[index][0]) || exec->args[index][0] == '_')
				&& !ft_strchr(exec->args[index], '='))
			ret = get_args_in_end(exec, exec->args[index]);
		else
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(exec->args[index], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			ret = 1;
		}
		index++;
	}
	return (ret);
}
