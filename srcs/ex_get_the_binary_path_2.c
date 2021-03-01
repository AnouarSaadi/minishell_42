/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_get_the_binary_path_2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 15:41:29 by asaadi            #+#    #+#             */
/*   Updated: 2021/03/01 11:38:04 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** the function "int check_if_executable(const char *filename)"
** Return non-zero if the name is an executable file, and
** zero if it is not executable, or if it does not exist.
*/

int     check_if_executable(const char *filename)
{
     int            result;
     struct stat    statinfo;
     
     result = stat(filename, &statinfo);
     if (result == -1)
        return (0);
     if (!S_ISREG(statinfo.st_mode))
        return (0);
     return (1);
}

/*
** int check_if_file_or_directory(char *av)
** check the argument is a file or directory with the 'stat' function.
** return non-zero if the file or directory is exist.
*/

int     check_if_file_or_directory(char *av)
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

/*
** char *check_pathvar(t_exec *exec) check the PATH is in envp or null
** return the value of PATH if is success, or NULL is does not exist.
*/

char    *check_pathvar(t_exec *exec)
{
    int     index;
    char    **sp;

    index = 0;
    while (exec->envp[index])
    {
        sp = ft_split(exec->envp[index], '=');
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
        index++;
    }
    return (NULL);
}

/*
** "char *concat_path_cmd(char *pathname, char **cmd)" is the function that concatenate
** the each binary path in PATH with command.
*/

char    *concat_path_cmd(char *pathname, char **cmd)
{
	char *bin;

	if (!(bin = (char *)malloc(sizeof(char) * (ft_strlen(pathname) + ft_strlen(*cmd) + 1))))
        return (NULL);
	ft_strlcpy(bin, pathname, ft_strlen(pathname) + 1);
	ft_strlcat(bin, "/", ft_strlen(bin) + 2);
	ft_strlcat(bin, *cmd, ft_strlen(bin) + ft_strlen(*cmd) + 1);
	return (bin);
}