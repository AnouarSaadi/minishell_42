/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 18:12:15 by asaadi            #+#    #+#             */
/*   Updated: 2021/03/04 12:54:34 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int sig)
{
	if (sig == SIGQUIT)
	{
		g_sig = CB;
		if (!g_var)
			ft_putstr_fd("\b\b  \b\b", 1);
	}
	else if (sig == SIGINT)
	{
		g_sig = CC;
		if (!g_var)
			ft_putstr_fd("\b\b  \n\033[0;33mminishell-42$ \033[0m", 1);
	}
}

void	get_return_signals(t_exec *exec)
{
	if (g_sig == CB && g_var)
	{
		ft_putendl_fd("^\\Quit: 3", 1);
		exec->code_ret = 131;
	}
	if (g_sig == CC && g_var)
	{
		// ft_putstr_fd("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b                  \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b", 1);
		ft_putendl_fd("^C", 1);
		exec->code_ret = 130;
	}
	g_sig = 0;
	g_var = 0;
}

void	handling_ctrl_d(t_exec *exec, char **line)
{
	char *tmp;
	char *tmp1;

	if (exec->r == 2)
	{
		exec->r = get_next_line(0, &tmp);
		tmp1 = *line;
		*line = ft_strjoin(tmp1, tmp);
		if (exec->r != 0)
			lexer(*line, exec);
		free(tmp);
		free(tmp1);
	}
	if (ft_strlen(*line) != 0 && exec->r == 0)
		exec->r = 2;
	else if (ft_strlen(*line) == 0)
		exit_func(exec, 1);
}
