/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 15:37:14 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/21 17:21:10 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int echo_function(char **args)
{
	int i;
	int del_newline;

	del_newline = 0;
	if (args[1] && !ft_strncmp(args[1], "-n", 2))
		del_newline = 1;
	i = (del_newline == 0) ? 1 : 2;
	while (!ft_strcmp(args[i], "-n"))
		i++;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1] != NULL)
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (del_newline == 0)
		write(1, "\n", 1);
	return (0);
}