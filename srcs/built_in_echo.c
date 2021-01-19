/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 15:37:14 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/19 18:19:49 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void echo_function(char *arg_to_print, int fd, int newline)
{
	if (arg_to_print)
	{
		if (newline == 0)
			ft_putendl_fd(arg_to_print, fd);
		else
			ft_putstr_fd(arg_to_print, fd);
	}
	else
	{
		if (newline == 0)
			write(1, "\n", 1);
	}
}