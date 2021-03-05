/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex_built_in_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 18:07:49 by asaadi            #+#    #+#             */
/*   Updated: 2021/03/05 17:58:52 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int			check_exit_arg(char *arg)
{
	int i;

	if (arg)
	{
		i = -1;
		if (!ft_strncmp(arg, "-", 1))
			i++;
		while (arg[++i])
			if (!ft_isdigit(arg[i]))
				return (0);
	}
	return (1);
}

static long long	get_result(int sign, char *str, int i)
{
	long long	res;
	long long	res__;

	while ((str[i] >= '0' && str[i] <= '9') && str[i])
	{
		res = res * 10 + (str[i] - '0');
		if (res < res__)
			break ;
		res__ = res;
		i++;
	}
	res = sign * res;
	if ((sign > 0 && res < 0) || (sign < 0 && res > 0) || !check_exit_arg(str))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(str, 2);
		ft_putendl_fd(": numeric argument required", 2);
		return (255);
	}
	return (res);
}

static long long	ft_exit_arg_lld(char *str)
{
	int			sign;
	int			i;

	if (!*str)
		return (0);
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
	return (get_result(sign, str, i));
}

int					exit_func(t_exec *exec, int ctrl_d)
{
	long long id;

	if (exec->pipe)
		return (0);
	ft_putendl_fd("exit", 1);
	if (!ctrl_d && exec->args && exec->args[1])
	{
		id = ft_exit_arg_lld(exec->args[1]);
		if (count_vars_env(exec->args) != 2 && id != 255)
		{
			ft_putendl_fd("minishell: exit: too many arguments", 2);
			return (1);
		}
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
