/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_get_envp_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 17:06:39 by asaadi            #+#    #+#             */
/*   Updated: 2021/03/06 10:38:00 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** char	*get_var_env(char **envp, char *var_to_check)
** search in envp and get the value of the var_to_check
** if exist or empty string if not exist.
*/

char	*get_var_env(char **envp, char *var_to_check)
{
	int		index;
	char	*val_to_ret;
	char	**equ_sp;
	char	*tmp;

	index = 0;
	val_to_ret = ft_strdup("");
	while (envp[index])
	{
		equ_sp = ft_split(envp[index], '=');
		if (!ft_strcmp(equ_sp[0], var_to_check))
		{
			tmp = ft_strchr(envp[index], '=');
			ft_free_arr((void**)&val_to_ret);
			val_to_ret = ft_strdup(tmp + 1);
		}
		ft_free_2dem_arr((void***)&equ_sp);
		index++;
	}
	return (val_to_ret);
}
