/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_envp_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 17:06:39 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/28 15:47:21 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_env(char **envp, char *var_to_check)
{
	int	count_vars;
	char *val_to_ret;
	char **equ_split;
	char *tmp;

	count_vars = 0;
	val_to_ret = ft_strdup("");
	while (envp[count_vars])
	{
		equ_split = ft_split(envp[count_vars], '=');
		if (ft_strncmp(equ_split[0], var_to_check, ft_strlen(equ_split[0])) == 0)
		{
			tmp = ft_strchr(envp[count_vars], '=');
			val_to_ret = ft_strdup(tmp + 1);
		}
		ft_free_2dem_arr(equ_split);
		count_vars++;
	}
	return (val_to_ret);
}
