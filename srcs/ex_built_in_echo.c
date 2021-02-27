/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_built_in_echo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 15:37:14 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/27 19:26:15 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo_function(char **args)
{
	int index;
	int del_newline;

	del_newline = 0;
	if (args[1] && !ft_strncmp(args[1], "-n", 2))
		del_newline = 1;
	if (del_newline == 0)
		index = 1;
	else
		index = 2;
	while (!ft_strncmp(args[index], "-n", 2))
		index++;
	while (args[index])
	{
		ft_putstr_fd(args[index], 1);
		if (args[index + 1] != NULL)
			ft_putchar_fd(' ', 1);
		index++;
	}
	if (del_newline == 0)
		ft_putchar_fd('\n', 1);
	return (0);
}
