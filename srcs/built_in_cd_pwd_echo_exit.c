/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd_pwd_echo_exit.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 15:22:13 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/09 12:37:47 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void pwd_function(void)
{
	char _cwd[PATH_MAX];

	if ((getcwd(_cwd, sizeof(_cwd)) == NULL))
		ft_putendl_fd(strerror(errno), 1);
	else
		ft_putendl_fd(_cwd, 1);
}

void echo_function(char *arg_to_print, int fd, int newline)
{
	if (newline == 0)
		ft_putendl_fd(arg_to_print, fd);
	else
		ft_putstr_fd(arg_to_print, fd);
}

void exit_function(int _id)
{
	exit(_id);
}