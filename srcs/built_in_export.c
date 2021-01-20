/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 19:08:16 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/20 19:26:42 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_function(char **envp, char *var_to_add)
{
	// char **new_envp;
	int len;
	// int i;
	// int j;
	(void)var_to_add;

	len  = count_vars_env(envp);
	printf("====>env_num %d\n", len);
	if (var_to_add)
	{
		envp[len] = var_to_add;
		envp[len + 1] = NULL;
	}
	// if (!(new_envp = (char **)malloc(sizeof(char*) * len + 2)))
	// 	return (NULL);
	// i = 0;
	// j = 0;
	// while (envp[i])
	// {
	// 	if (strncmp(envp[i], "_", 1) == 0)
	// 	{
	// 		if (var_to_add)
	// 		{
	// 			new_envp[j++] = var_to_add;
	// 			var_to_add = NULL;
	// 		}
	// 	}
	// 		new_envp[j++] = envp[i];
	// 	i++;
	// }
	// new_envp[j] = NULL;
    // i = 0;
    // while(new_envp[i])
    // {
    //     puts(new_envp[i]);
    //     i++;
    // }
	// return (envp);
}

void	print_envp(char **envp)
{
	int i;
	char *arg;
	char **equ;
	char *s_chr;

	i = 0;
	while(envp[i])
	{
		equ = ft_split(envp[i], '=');
		if((s_chr = ft_strchr(envp[i], '=')) != NULL)
		{
			arg = ft_strjoin("declare -x ", equ[0]);
			arg = ft_strjoin(arg, "=");
			arg = ft_strjoin(arg, "\"");
			arg = ft_strjoin(arg, s_chr + 1);
			arg = ft_strjoin(arg, "\"");
		}
		else
		{
			arg = ft_strjoin("declare -x ", equ[0]);
		}
		ft_putendl_fd(arg, 1);
		ft_free_2dem_arr(equ);
		ft_free_arr(arg);
		i++;
	}
}

void	sort_envp_alpha(char **envp)
{
	char *tmp;
	int i;
	int j;

	i = 0;
	j = 0;
	while(envp[i])
	{
		j = 0;
		while(envp[j])
		{
			if(ft_strncmp(envp[i], envp[j], ft_strlen(envp[i])) < 0)
			{
				tmp = envp[j];
				envp[j] = envp[i];
				envp[i] = tmp;
			}
			j++;
		}
		i++;
	}
	print_envp(envp);
}