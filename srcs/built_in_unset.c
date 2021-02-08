/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 18:12:22 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/08 19:38:18 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"minishell.h"

int 	count_vars_env(char **env_list)
{
	int count;

	count = 0;
	if (!env_list)
		return (0);
	while (env_list[count])
		count++;
	return (count);
}

void    get_args_in_end(t_exec *exec, char *var)
{
    int i;
    int j;
    char **equ;
    char **env;
    
    if (var)
    {
        if (!(env = (char **)malloc(sizeof(char *) * count_vars_env(exec->envp))))
        {
            ft_putendl_fd("ERROR ALLOCATION", 2);
            exit (1);
        }
        i = 0;
        j = 0;
        while((exec->envp)[i])
        {
            equ = ft_split((exec->envp)[i], '=');
            if (ft_strcmp(equ[0], var))
                env[j++] = ft_strdup((exec->envp)[i]);
            i++;
            ft_free_2dem_arr((void***)&equ);
        }
        env[j] = NULL;
        ft_free_2dem_arr((void***)&(exec->envp));
        exec->envp = env;
    }
}

void   unset_function(t_exec *exec)
{
	int i;

    i = 1;
    while (exec->args[i])
    {
        if ((ft_isalpha(exec->args[i][0]) || exec->args[i][0] == '_') && !ft_strchr(exec->args[i], '='))
            get_args_in_end(exec, exec->args[i]);
        else
        {
            ft_putstr_fd("bash: unset: `", 2);
            ft_putstr_fd(exec->args[i], 2);
            ft_putendl_fd("': not a valid identifier", 2);
        } 
        i++;
    }
}