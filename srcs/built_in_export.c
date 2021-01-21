/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 19:08:16 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/21 16:16:55 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void edit_in_envp(char **envp, char *var_to_edit)
{
	// puts(var_to_edit);
	if((ft_strrchr(var_to_edit, '=')))
		*envp = var_to_edit;
}

int len_to_char(char *str,int c)
{
	int i;

	i = 0;
	while (str[i])
	{
		if(str[i] == c)
			return (i);
		i++;
	}	
	return (i);
}

void	export_function(char **envp, char *var_to_add)
{
	int len;
	int edit;
	int i;
	// char **equ;

	len  = count_vars_env(envp);
	printf("==>env_num %d\n", len);
	i = 0;
	edit = 0;
	while(envp[i] && var_to_add)
	{
		if (ft_strncmp(envp[i], var_to_add, len_to_char(envp[i], '=')) == 0)
		{
			puts(var_to_add);
			puts(envp[i]);
			edit_in_envp(&envp[i], var_to_add);
			edit = 1;
		}
		i++;
	}
	if (edit == 1)
	{
		envp[len] = NULL;
	}
	if (var_to_add && edit == 0)
	{
		envp[len] = var_to_add;
		envp[len + 1] = NULL;
	}
// 	i = 0;
// 	while(envp[i])
// 	{
// 		puts(envp[i]);
// 		i++;
// 	}
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