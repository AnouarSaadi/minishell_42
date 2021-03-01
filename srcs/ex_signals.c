/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 18:12:15 by asaadi            #+#    #+#             */
/*   Updated: 2021/03/01 19:06:32 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void sig_handler(int sig)
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

void get_return_signals(t_exec *exec)
{
	if (g_sig == CB && g_var)
	{
		ft_putendl_fd("^\\Quit: 3", 1);
		exec->code_ret = 131;
	}
	if (g_sig == CC && g_var)
	{
		ft_putendl_fd("\n^C", 1);
		exec->code_ret = 130;
	}
	g_sig = 0;
	g_var = 0;
}

void handling_ctrl_d(t_exec *exec, char **line)
{
	// char *tmp;
	// char *tmp1;
	(void)exec;
	(void)line;
	// int i;
	// //printf("line ---- %zu\n", ft_strlen(line));
	// if (exec->r == 2)
	// {
	// 	exec->r = get_next_line(0, &tmp);
	// 	tmp1 = *line;
	// 	*line = ft_strjoin(tmp1, tmp);
	// 	if (exec->r == 0)
	// 	{
	// 		i = 0;
	// 		while ((size_t)i < ft_strlen(tmp1))
	// 		{
	// 			ft_putstr_fd("\b\b  \b\b", 1);
	// 			// ft_putstr_fd("\b ", 1);
	// 			i++;
	// 		}
	// 		ft_putstr_fd(*line, 1);
	// 	}
	// 	if (exec->r != 0)
	// 		lexer(*line, exec);
	// 	free(tmp);
	// 	free(tmp1);
	// 	//exec.r = get_next_line(0, &line);
	// }
	// if (ft_strlen(*line) != 0 && exec->r == 0)
	// 	exec->r = 2;
	// else if (ft_strlen(*line) == 0)
	// {
	// 	// puts("exit");
	// 	ft_putendl_fd("exit", 1);
	// 	// exit_func(exec->args);
	// }

	// // if (ft_strlen(line))
	// // 	return ;
}
