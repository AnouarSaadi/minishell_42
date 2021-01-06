/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_envp_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/02 10:10:50 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/06 18:03:44 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*check_var_env(char **envp, char *var_to_check)
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
		ft_free_split(equ_split);
		count_vars++;
	}
	return (val_to_ret);
}
