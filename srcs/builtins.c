/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 16:34:10 by asaadi            #+#    #+#             */
/*   Updated: 2020/12/25 17:06:46 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	_pwd_function(int fd)
{
	char *_pwd;

	_pwd = NULL;
	_pwd = getcwd(_pwd, 100);
	ft_putendl_fd(_pwd, fd);
}

void		_change_directory(char *_path)
{
	char *_pwd;
	char *_new_pwd;
	
	_pwd = NULL;
	_new_pwd = NULL;
	_pwd = getcwd(_pwd, 100);
	if (chdir(_path) == -1)
			ft_putendl_fd(strerror(errno), 1);
	_new_pwd = getcwd(_new_pwd, 100);
}