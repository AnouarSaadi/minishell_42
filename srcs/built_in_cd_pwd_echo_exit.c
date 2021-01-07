/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd_pwd_echo.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 15:22:13 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/07 15:23:00 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd_function(void)
{
	char _pwd[PATH_MAX];

	if ((getcwd(_pwd, sizeof(_pwd)) == NULL))
		ft_putendl_fd(strerror(errno), 1);
	else
		ft_putendl_fd(_pwd, 1);
}

void		change_directory(char *_path)
{
	if (chdir(_path) == -1)
			ft_putendl_fd(strerror(errno), 1);
}

void	echo_function(char *arg_to_print,int fd ,int newline)
{
	if (newline == 0)
		ft_putendl_fd(arg_to_print, fd);
	else
		ft_putstr_fd(arg_to_print, fd);
}

void	exit_function(int _id)
{
	exit(_id);
}