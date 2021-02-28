/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_built_in_pwd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 15:22:13 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/27 19:33:06 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** int pwd_function(void)
** function print the working directory
** return zero means the successful execution
*/

int	pwd_function(void)
{
	char cwd_[PATH_MAX];

	if ((getcwd(cwd_, sizeof(cwd_)) == NULL))
	{
		ft_putstr_fd("minishell: pwd: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (1);
	}
	else
		ft_putendl_fd(cwd_, 1);
	return (0);
}
