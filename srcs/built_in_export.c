/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 19:08:16 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/25 14:38:14 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_envp(char **envp);

int len_to_char(char *str, int c)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (i);
}

char *seach_env(char **envp, char *str)
{
	int i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], str, len_to_char(str, '=')))
			return (str);
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
		if (!ft_strncmp(envp[i], var_to_edit, len_to_char(var_to_edit, '=')))
		{
			envp[i] = ft_strdup(var_to_edit);
			break;
		}
		i++;
	}
}

// char **add_var(char **envp, char *var, int len)
// {
// 	char	**env__p;
// 	// int		len;

// 	if (!(env__p = (char **)malloc(sizeof(char *) * (len + 1))))
// 		ft_putendl_fd("ERROR ALLOCATION!", 1);
// 	int  i = 0;
// 	while (envp[i])
// 	{
// 		env__p[i] = ft_strdup(envp[i]);
// 		i++;
// 	}
// 	env__p[i++] = ft_strdup(var);
// 	env__p[i] = NULL;
// 	envp = env__p;
// 	// envp = envp_cpy(env__p);
// 	// return(envp);
// }

void export_function(char ***envp, char *var_to_add)
{
	int len;
	int i;
	char	**env__p;

	// var_to_add = ft_strtrim(var_to_add, "\n");
	i = 0;
	env__p = NULL;
	len = count_vars_env(envp[0]);
	if (seach_env(envp[0], var_to_add))
		edit_in_envp(envp[0], var_to_add);
	else if (var_to_add)
	{
		if (!(env__p = (char **)malloc(sizeof(char *) * (len + 1))))
			ft_putendl_fd("ERROR ALLOCATION!", 1);
		// i = 0;
		// while (envp[0][i])
		// {
		// 	puts(envp[0][i]);
		// 	i++;
		// }
		printf("len of envp {%d}\n", len);
		i = 0;
		while (envp[0][i])
		{
			env__p[i] = ft_strdup(envp[0][i]);
			i++;
		}
		// if (envp[i] == NULL)
		env__p[i++] = ft_strdup(var_to_add);
		env__p[i] = NULL;
		printf("len of new_envp {%d}\n", i);
		envp[0] = env__p;
	}
	i = 0;
	while (env__p[i])
	{
		puts(env__p[i]);
		i++;
	}
}

void print_envp(char **envp)
{
	char **equ;
	char *s_chr;
	int i;

	i = 0;
	while (envp[i])
	{
		equ = ft_split(envp[i], '=');
		if ((s_chr = ft_strchr(envp[i], '=')) != NULL)
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

void sort_print_envp_alpha(char **envp)
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
	while (str[i])
	{
		j = 0;
		while (str[j])
		{
			if (ft_strncmp(str[i], str[j], ft_strlen(str[i])) < 0)
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
	// print_envp(str);
}