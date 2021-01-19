/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 19:08:16 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/19 18:23:43 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_function(char **envp, char *var_to_add)
{
	// char **export_env;
	// int	var_num;
	int i;
	// int j;
	(void)var_to_add;
	i=0;
	while (envp[i])
	{
		puts(envp[i]);
		i++;
	}
	
	// var_num = count_vars_env(*envp);
	// if (var_to_add && ft_strchr(var_to_add, '='))
	// 	var_num += 2;
	// if (!(export_env = (char**)malloc(sizeof(char*) + var_num)))
	// 	exit(1);
	// j = 0;
	// if (var_to_add && ft_strchr(var_to_add, '='))
	// {
	// 	if (!(export_env[j] = (char*)malloc(sizeof(char) * ft_strlen(var_to_add) + 1)))
	// 		exit(1);
	// 	ft_strlcpy(export_env[j], var_to_add, ft_strlen(var_to_add) + 1);
	// 	j++;
	// }
	// i = 0;
	// while (*envp[i])
	// {
	// 	if (!(export_env[j] = (char*)malloc(sizeof(char) * ft_strlen(*envp[i]) + 1)))
	// 		exit(1);
	// 	ft_strlcpy(export_env[j], *envp[i], ft_strlen(*envp[i]) + 1);
	// printf("%d\n", var_num);
	// 	i++;
	// 	j++;
	// 	puts(*envp[i]);
	// }
	// export_env[j] = NULL;
	// i = 0;
	// while(export_env[i])
	// {
	// 	puts(export_env[i]);
	// 	i++;
	// }
}
	// int len;
	// int i;
	// int j;
	// char **new_envp;

	// i = 0;
	// j = 0;
	// len  = count_vars_env(*envp);
	// new_envp = NULL;
	// if (var_to_add && ft_strchr(var_to_add, '='))
	// {
	// 	if (!(new_envp = (char **)malloc(sizeof(char*) * len + 1)))
	// 		exit(1);
	// 	while(*envp[i])
	// 	{
	// 		new_envp[i] = malloc(sizeof(char) * ft_strlen(*envp[i]) + 1);
	// 		ft_strlcpy(new_envp[i], *envp[i], ft_strlen(*envp[i]));
	// 	puts("++++++++");
	// 		i++;
	// 	}
	// 	if (var_to_add && ft_strchr(var_to_add, '='))
	// 	{
	// 		new_envp[i++] = ft_strdup(var_to_add);
	// 		var_to_add = NULL;
	// 	}
	// 	new_envp[i] = NULL;
	// }
	// *envp = new_envp;
	// *envp[i] = NULL;