/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_the_binary_path.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 17:06:58 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/19 18:24:54 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *check_pathvar(t_exec *exec)
{
    int cnt;
    char **sp;

    cnt = 0;
    while (exec->envp[cnt])
    {
        sp = ft_split(exec->envp[cnt], '=');
        if (!ft_strncmp(sp[0], "PATH", ft_strlen(sp[0])))
        {
            if (!ft_strlen(sp[1]))
            {
                ft_free_2dem_arr((void***)&sp);
                return (NULL);
            }
            ft_free_2dem_arr((void***)&sp);
            return (get_var_env(exec->envp, "PATH"));
        }
        ft_free_2dem_arr((void***)&sp);
    }
    return (NULL);
}

int			get_cmd_binary_path(t_exec *exec)
{
    char *path_env;
    char **sp;

    path_env = check_pathvar(exec);
    if (!path_env){};
    //when you are done with theb path_env free it
}