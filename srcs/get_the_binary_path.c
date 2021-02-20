/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_the_binary_path.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 17:06:58 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/20 16:35:01 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Return non-zero if the name is an executable file, and
** zero if it is not executable, or if it does not exist.
*/

int check_if_executable(const char *filename)
{
     int result;
     struct stat statinfo;
     
     result = stat(filename, &statinfo);
     if (result < 0)
        return (0);
     if (!S_ISREG(statinfo.st_mode))
        return (0);
     return (statinfo.st_mode & S_IXOTH);
}

int check_if_file_or_directory(char *av)
{
    struct stat statinfo;
    int         ret;

    ret = stat(av, &statinfo);
    if (ret == 0 && (S_IFDIR & statinfo.st_mode))
        return(IS_DIR);
    else if (ret == 0 && (S_IFREG & statinfo.st_mode))
        return(IS_FILE);
    return (0);
}

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
        cnt++;
    }
    return (NULL);
}

char *concat_path_cmd(char *pathname, char **cmd)
{
	char *bin;

	if (!(bin = (char *)malloc(sizeof(char) * (ft_strlen(pathname) + ft_strlen(*cmd) + 1))))
        return (NULL);
	ft_strlcat(bin, pathname, ft_strlen(pathname) + 1);
	ft_strlcat(bin, "/", ft_strlen(bin) + 2);
	ft_strlcat(bin, *cmd, ft_strlen(bin) + ft_strlen(*cmd) + 1);
	return (bin);
}

int print_message(char *bin, char *msg, t_exec *exec, int code)
{
    ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(exec->args[0], 2);
	ft_putendl_fd(msg, 2);
    ft_free_arr((void**)&bin);
    exec->code_ret = code;
    return (0);
}

int check__executable(t_exec *exec)
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
        return (print_message(NULL, ": command not found", exec, 127));
    else
    {
        bin = concat_path_cmd(getcwd(NULL, PATH_MAX), exec->args);
        bin_exec = get_var_env(exec->envp, "_");
        if (!ft_strcmp(bin, bin_exec))
        {
            ft_free_arr((void**)&(exec->args[0]));
            exec->args[0] = ft_strdup(bin);
        }
        ft_free_arr((void**)&bin);
        ft_free_arr((void**)&bin_exec);
    }
    return (1);
}

int         check__slash(t_exec *exec)
{
    if (check_if_file_or_directory(exec->args[0]) == 0)
        return (print_message(NULL, ": No such file or directory", exec, 127));
    return (1);
}

int			get_cmd_binary_path(t_exec *exec)
{
    char *path_env;
    char **sp;
    char *bin;
    int i;
    
    if ((check_if_file_or_directory(exec->args[0]) == IS_DIR) && ft_strchr(exec->args[0], '/'))
        return (print_message(NULL, ": is a directory", exec, 126));
    if (!ft_strncmp(exec->args[0], "/", 1))
        return (check__slash(exec));
    if (!ft_strncmp(exec->args[0], ".", 1))
        return (check__executable(exec));
    path_env = check_pathvar(exec);
    if (!path_env)
        exit(0);
    else
    {
        sp = ft_split(path_env, ':');
        i = 0;
        while (sp[i])
        {
            bin = concat_path_cmd(sp[i], exec->args); // check if the func is failed
            if (check_if_executable(bin))
                break;
            i++;
        }
        ft_free_2dem_arr((void***)&sp);
        ft_free_arr((void**)&path_env);
        if (check_if_file_or_directory(bin) == 0)
            return (print_message(bin, ": command not found", exec, 127));
        else
        {
            ft_free_arr((void**)&(exec->args[0]));
            exec->args[0] = ft_strdup(bin);
            return (1);
        }
    }
    return (0);
}