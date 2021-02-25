/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 19:08:16 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/25 14:53:11 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *seach_env(char **envp, char *str)
{
	int		index;
	char	**equ0;
	char	**equ1;

	index = 0;
	while (envp[index])
	{
		equ0 = ft_split(envp[index], '=');
		equ1 = ft_split(str, '=');
		if (!ft_strcmp(equ0[0], equ1[0]))
		{
			ft_free_2dem_arr((void***)&equ0);
			ft_free_2dem_arr((void***)&equ1);
			return (str);
		}
		index++;
		ft_free_2dem_arr((void***)&equ0);
		ft_free_2dem_arr((void***)&equ1);
	}
	return (NULL);
}

/*
** the "void edit_in_envp(char **envp, char *var_to_edit)" function is used to edit in vars at envp.
*/

static void edit_in_envp(char **envp, char *var_to_edit)
{
	int index;
	char **sp;
	char **sp_var;

	index = 0;
	sp_var = ft_split(var_to_edit, '=');
	while (envp[index])
	{
		sp = ft_split(envp[index], '=');
		if (!ft_strcmp(*sp, *sp_var) && ft_strcmp(*sp, "_"))
		{
			ft_free_arr((void**)&(envp[index]));
			ft_free_2dem_arr((void***)&sp);
			envp[index] = ft_strdup(var_to_edit);
			break;
		}
		ft_free_2dem_arr((void***)&sp);
		index++;
	}
	ft_free_2dem_arr((void***)&sp_var);
}

/*
** the function "void export_function(t_exec *exec)"
** function of exporting the vars or editing to envp.
*/

static int export_func_2(t_exec *exec, char *arg)
{
	char **env__p;
	int index;

	if (ft_strchr(arg, '=') && seach_env(exec->envp, arg))
		edit_in_envp(exec->envp, arg);
	else if (arg && !seach_env(exec->envp, arg))
	{
		if (!(env__p = malloc(sizeof(char *) * (count_vars_env(exec->envp) + 2))))
		{
			ft_putendl_fd("minishell: Error: failed to allocation memory.", 2);
			return (1);
		}
		index = -1;
		while (exec->envp[++index])
			env__p[index] = ft_strdup(exec->envp[index]);
		env__p[index] = ft_strdup(arg);
		env__p[index + 1] = NULL;
		ft_free_2dem_arr((void***)&(exec->envp));
		exec->envp = env__p;
	}
	return (0);
}

int export_function(t_exec *exec)
{
	int index;
	int ret;

	ret = 0;
	if (!exec->args[1])
		return(sort_print_envp_alpha(exec->envp));
	index = 1;
	while (exec->args[index])
	{
		if ((ft_isalpha(exec->args[index][0]) || exec->args[index][0] == '_'))
			ret = export_func_2(exec, exec->args[index]);
		else
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(exec->args[index], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			ret = 1;
		}
		index++;
	}
	return(ret);
}
