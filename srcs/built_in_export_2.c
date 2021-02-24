/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 15:53:46 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/24 17:00:12 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** this two functions "void sort_print_envp_alpha(char **envp)" "void print_envp(char **envp)"
** are used to print envp with alpaha_order in export without any arguments.
*/

static void print_envp(char **envp)
{
	char **equ;
	char *s_chr;
	int index;

	index = -1;
	while (envp[++index])
	{
		equ = ft_split(envp[index], '=');
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(equ[0], 1);
		if ((s_chr = ft_strchr(envp[index], '=')) != NULL)
		{
			ft_putstr_fd("=", 1);
			ft_putstr_fd("\"", 1);
			ft_putstr_fd(s_chr + 1, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putchar_fd('\n', 1);
		ft_free_2dem_arr((void***)&equ);
	}
}

int sort_print_envp_alpha(char **envp)
{
	int index[2];
	char **str;
	char *tmp;

	str = envp_cpy(envp);
	index[0] = -1;
	while (str[++index[0]])
	{
		index[1] = -1;
		while (str[++index[1]])
		{
			if (ft_strcmp(str[index[0]], str[index[1]]) < 0)
			{
				tmp = str[index[1]];
				str[index[1]] = str[index[0]];
				str[index[0]] = tmp;
			}
		}
	}
	print_envp(str);
	ft_free_2dem_arr((void***)&str);
	return (0);
}
