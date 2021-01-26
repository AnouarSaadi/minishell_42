/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 18:12:22 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/26 15:14:41 by asaadi           ###   ########.fr       */
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

void    get_args_in_end(char ***e, char *var)
{
    int i;
    int j;
    char **equ;
    char **env;
    
    if (var)
    {
        if (!(env = (char **)malloc(sizeof(char *) * count_vars_env(*e))))
        {
            ft_putendl_fd("ERROR ALLOCATION", 2);
            exit (1);
        }
        i = 0;
        j = 0;
        while((*e)[i])
        {
            equ = ft_split((*e)[i], '=');
            if (ft_strcmp(equ[0], var))
                env[j++] = ft_strdup((*e)[i]);
            i++;
            ft_free_2dem_arr(equ);
        }
        env[j] = NULL;
        ft_free_2dem_arr(*e);
        *e = env;
    }
}

void   unset_function(char ***envp, char **args)
{
	int i;

    i = 1;
    while (args[i])
    {
        if ((ft_isalpha(args[i][0]) || args[i][0] == '_') && !ft_strchr(args[i], '='))
            get_args_in_end(envp, args[i]);
        else
        {
            ft_putstr_fd("bash: unset: `", 2);
            ft_putstr_fd(args[i], 2);
            ft_putendl_fd("': not a valid identifier", 2);
        } 
        i++;
    }
}