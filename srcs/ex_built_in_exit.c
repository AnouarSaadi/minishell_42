/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_built_in_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 18:07:49 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/28 18:16:50 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_func(t_exec *exec, t_list *tl, t_list *cond_list)
{
	int id;

	if (!exec->args[1])
		id = 0;
	else
		id = ft_atoi(exec->args[1]);
	exec->code_ret = id;
	if (exec->envp)
		ft_free_2dem_arr((void***)&(exec->envp));
	if (tl)
		free_tokens_list(tl);
	if (cond_list)
		free_list(cond_list);
	exit(id);
}