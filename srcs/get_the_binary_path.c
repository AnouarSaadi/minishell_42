/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_the_binary_path.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 17:06:58 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/24 17:00:00 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** int print_error(char *bin, char *msg, t_exec *exec, int code)
*/

static int print_error(char *bin, char *msg, t_exec *exec, int code)
{
    ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(exec->args[0], 2);
	ft_putendl_fd(msg, 2);
    ft_free_arr((void**)&bin);
    exec->code_ret = code;
    return (0);
}

/*
** int check__executable(t_exec *exec)
** check the file 
*/

static int check__executable(t_exec *exec)
{
    char *bin;
    char *bin_exec;

    if (ft_strlen(exec->args[0]) == 1)
    {
		ft_putendl_fd("bash: .: filename argument required", 2);
		ft_putendl_fd(".: usage: . filename [arguments]", 2);
		exec->code_ret = 2;
		return (0);
    }
    else if (ft_strlen(exec->args[0]) > 1 && !ft_strchr(exec->args[0], '/'))
        return (print_error(NULL, ": command not found", exec, 127));
    if (!(bin = concat_path_cmd(getcwd(NULL, PATH_MAX), exec->args)))
        return (print_error(NULL, ": Failed to allocate memory", NULL, 127));
    bin_exec = get_var_env(exec->envp, "_");
    if (!ft_strcmp(bin, bin_exec))
    {
        ft_free_arr((void**)&(exec->args[0]));
        exec->args[0] = ft_strdup(bin);
    }
    ft_free_arr((void**)&bin);
    ft_free_arr((void**)&bin_exec);
    return (1);
}

/*
** int     searchinpath(t_exec *exec, char **path_env)
** search in PATH and get the bunary path of command.
*/

static int     searchinpath(t_exec *exec, char **path_env)
{
    char **sp;
    char *bin;
    int index;

    sp = ft_split((*path_env), ':');
    index = -1;
    while (sp[++index])
    {
        if (!(bin = concat_path_cmd(sp[index], exec->args)))
        {
            ft_free_2dem_arr((void***)&sp);
            return (print_error(NULL, ": Failed to allocate memory", NULL, 127));
        }
        if (check_if_executable(bin))
            break;
    }
    ft_free_2dem_arr((void***)&sp);
    ft_free_arr((void**)&(*path_env));
    if (check_if_file_or_directory(bin) == 0)
        return (print_error(bin, ": command not found", exec, 127));
    ft_free_arr((void**)&(exec->args[0]));
    exec->args[0] = ft_strdup(bin);
    return (1);
}

/*
** The function "int get_cmd_binary_path(t_exec *exec)" is work to find the binary path for the command.
*/

int			get_cmd_binary_path(t_exec *exec)
{
    char *path_env;
    
    if ((check_if_file_or_directory(exec->args[0]) == IS_DIR) &&
        ft_strchr(exec->args[0], '/'))
        return (print_error(NULL, ": is a directory", exec, 126));
    if (!ft_strncmp(exec->args[0], "/", 1) &&
        !check_if_file_or_directory(exec->args[0]))
        return (print_error(NULL, ": No such file or directory", exec, 127));
    if (!ft_strncmp(exec->args[0], ".", 1))
        return (check__executable(exec));
    if (!(path_env = check_pathvar(exec)))
        return (print_error(NULL, ": No such file or directory", exec, 127));
    return (searchinpath(exec, &path_env));
}
