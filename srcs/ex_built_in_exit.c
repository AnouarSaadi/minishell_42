/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_built_in_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 18:07:49 by asaadi            #+#    #+#             */
/*   Updated: 2021/03/04 10:43:55 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long long	ft_long_long(long long res, int sign)
{
	if (res > 0 && sign < 0)
		return (-1);
	if (res < 0 && sign > 0)
		return (-2);
	return (res);
}

static long long	ft_exit_arg(const char *str)
{
	long long	res;
	int			sign;
	int			i;

	if (!*str)
		return (0);
	res = 0;
	sign = 1;
	i = 0;
	while ((str[i] == '\t' || str[i] == '\n' || str[i] == '\v' ||
				str[i] == '\f' || str[i] == '\r' || str[i] == ' ') && str[i])
		i++;
	if ((str[i] == '-' || str[i] == '+') && str[i])
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while ((str[i] >= '0' && str[i] <= '9') && str[i])
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	res = sign * res;
	return (ft_long_long(res, sign));
}

static void			ft_exit_fin(long long *id, t_exec *exec)
{
	if (*id < 0)
	{
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(exec->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		*id = 255;
	}
}

int					exit_func(t_exec *exec, int ctrl_d)
{
	long long id;

	ft_putendl_fd("exit", 1);
	id = 0;
	if (!ctrl_d && exec->args && exec->args[1])
	{
		id = ft_exit_arg(exec->args[1]);
		ft_exit_fin(&id, exec);
	}
	else
		id = exec->code_ret;
	if (!ctrl_d && exec->args)
		ft_free_2dem_arr((void***)&(exec->args));
	if (exec->envp)
		ft_free_2dem_arr((void***)&(exec->envp));
	if (!ctrl_d && exec->tl != NULL)
		free_tokens_list(exec->tl);
	if (!ctrl_d && exec->cond != NULL)
		free_list(exec->cond);
	exit(id);
}
