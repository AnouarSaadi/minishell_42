/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 09:42:53 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/07 12:51:33 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <sys/types.h>
# include <fcntl.h>
# include "libft.h"
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <signal.h>
# include <dirent.h>
# include <sys/stat.h>
# define PATH_MAX 1024

void		change_directory(char *_path);
void		pwd_function(void);
void		env_function(char **envp);
int			count_vars_env(char **env_list);
char		*check_var_env(char **envp, char *var_to_check);
void		ft_free_2dem_arr(char **arr);
void	find_the_cmd_path(char **args, char **envp);
void		ft_free_arr(void *arr);

#endif