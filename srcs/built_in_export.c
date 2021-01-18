/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 19:08:16 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/18 19:33:32 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**export_function(char **envp, char *var_to_add)
{
	char **new_envp;
	int len;
	int i;
	int j;

    puts(var_to_add);
	len  = count_vars_env(envp);
	if (!(new_envp = (char **)malloc(sizeof(char*) * len + 2)))
		return (NULL);
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (strncmp(envp[i], "_", 1) == 0)
		{
            puts(var_to_add);
			new_envp[j++] = var_to_add;
			new_envp[j++] = envp[i];
		}
		else
			new_envp[j++] = envp[i];
		i++;
	}
	new_envp[j] = NULL;
    // i = 0;
    // while(new_envp[i])
    // {
    //     puts(new_envp[i]);
    //     i++;
    // }
	return (new_envp);
}