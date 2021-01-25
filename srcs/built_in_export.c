/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 19:08:16 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/25 19:24:08 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void export_function(char ***e, char **args)
{
	int len;
	int i;
	char	**env__p;
	int j;

	j = 1;
	while (args[j])
	{
		if (ft_isalpha(args[j][0]) || args[j][0] == '_')
		{
			if (seach_env(*e, args[j]))
				edit_in_envp(*e, args[j]);
			else if (args[j])
			{
				len = count_vars_env(*e);
				if (!(env__p = (char **)malloc(sizeof(char *) * (len + 2))))
				{
					ft_putendl_fd("ERROR ALLOCATION!", 2);
					exit(EXIT_FAILURE);
				}
				i = 0;
				while ((*e)[i])
				{
					env__p[i] = ft_strdup((*e)[i]);
					i++;
				}
				env__p[i] = args[j];
				env__p[i + 1] = NULL;
				*e = env__p;
			}
		}
		else
		{
			ft_putstr_fd("bash: export: `", 2);
            ft_putstr_fd(args[j], 2);
            ft_putendl_fd("': not a valid identifier", 2);
		}
		j++;
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
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(equ[0], 1);
		if ((s_chr = ft_strchr(envp[i], '=')) != NULL)
		{
			ft_putstr_fd("=", 1);
			ft_putstr_fd("\"", 1);
			ft_putstr_fd(s_chr + 1, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putchar_fd('\n', 1);
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
			}
			j++;
		}
		i++;
	}
	print_envp(str);
}