/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 19:08:16 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/23 12:28:38 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		print_envp(char **envp);

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

char *seach_env(char **envp,char *str)
{
	int i = 0;
	while (envp[i])
	{
		if(!ft_strncmp(envp[i],str,len_to_char(str,'=')))
			return str;
		i++;
	}
	return NULL;
}

void edit_in_envp(char **envp, char *var_to_edit)
{
	int i;

	i = 0;
	while (envp[i])
	{
		if(!ft_strncmp(envp[i], var_to_edit, len_to_char(var_to_edit,'=')))
		{
			envp[i] = var_to_edit;
			break;
		}
		i++;
	}
}

void	export_function(char **envp, char *var_to_add)
{
	int len;
	int edit;
	int i;

	len  = count_vars_env(envp);
	// ft_putnbr_fd(len,1);
	// ft_putendl_fd("",1);
	i = 0;
	edit = 0;
	if (seach_env(envp,var_to_add))
		edit_in_envp(envp, var_to_add);
	else if (var_to_add)
	{
		// printf("|%s|\n", var_to_add);
		envp[len] = var_to_add;
		envp[len + 1] = NULL;
		print_envp(envp);
	}
	// print_envp(envp);
	// i = 0;
	// while (envp[i])
	// {
	// 	puts(envp[i]);
	// 	//printf("{%s}\n", envp[i]);
	// 	i++;
	// 	// 
	// }
	// len  = count_vars_env(envp);
	// ft_putnbr_fd(len,1);
	// ft_putendl_fd("",1);

}

void	print_envp(char **envp)
{
	char **equ;
	char *s_chr;
	int i;

	i = 0;
	while(envp[i])
	{
		equ = ft_split(envp[i], '=');
		if((s_chr = ft_strchr(envp[i], '=')) != NULL)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(equ[0], 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd("\"", 1);
			ft_putstr_fd(s_chr + 1, 1);
			ft_putendl_fd("\"", 1);
		}
		else
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putendl_fd(equ[0], 1);
		}
		ft_free_2dem_arr(equ);
		i++;
	}
}


void	sort_print_envp_alpha(char **envp)
{
	char *tmp;
	int i;
	int j;
	char **str;

	i = 0;
	j = 0;
	str = envp_cpy(envp);
	// print_envp(str);
	i = 0;
	while(str[i])
	{
		j = 0;
		while(str[j])
		{
			if(ft_strncmp(str[i], str[j], ft_strlen(str[i])) < 0)
			{
				tmp = str[j];
				str[j] = str[i];
				str[i] = tmp;
				// ft_putendl_fd(tmp, 1);
				// tmp = NULL;
			}
			j++;
		}
		i++;
	// ft_putendl_fd(str[i], 1);
	}
	// str[i] = NULL;
	// i = 0;
	// while(str[i])
	// {
	// 	puts(str[i]);
	// 	i++;
	// }
	print_envp(str);
}