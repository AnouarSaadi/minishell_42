/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 19:08:16 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/22 17:24:13 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
		if(!ft_strncmp(envp[i],var_to_edit,len_to_char(var_to_edit,'=')))
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
	// char **equ;

	// var_to_add = ft_strtrim(var_to_add, "\n");
	len = 0;
	len  = count_vars_env(envp);
	// printf("%d",len);
	ft_putnbr_fd(len,1);
	ft_putendl_fd("",1);
	// printf("==>{%s}\n", var_to_add);
	i = 0;
	edit = 0;
	if (seach_env(envp,var_to_add))
		edit_in_envp(envp, var_to_add);
	else if (var_to_add)
	{
		ft_putendl_fd(var_to_add, 1);
		envp[len] = ft_strdup(var_to_add);
		envp[len + 1] = NULL;
	}
	len  = count_vars_env(envp);
	ft_putnbr_fd(len,1);
	ft_putendl_fd("",1);

}

void	print_envp(char **envp)
{
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
			arg = ft_strjoin("declare -x ", equ[0]);
		ft_putendl_fd(arg, 1);
		ft_free_2dem_arr(equ);
		ft_free_arr(arg);
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
	i = 0;
	while(str[i])
	{
		j = 0;
		while(str[j])
		{
			if(ft_strncmp(str[i], str[j], ft_strlen(str[i])) < 0)
			{
				tmp = str[i];
				str[i] = str[j];
				str[j] = tmp;
			}
			j++;
		}
		i++;
	ft_putendl_fd(str[i], 1);
	}
	// str[i] = NULL;
	// i = 0;
	// while(str[i])
	// {
	// 	puts(str[i]);
	// 	i++;
	// }
	// // print_envp(str);
}