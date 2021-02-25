/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/23 18:01:09 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/25 14:47:54 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** int execve_failure(char *arg, char *err_msg)
** function use if execve failed
*/

int execve_failure(char *arg, char *err_msg)
{
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(arg, 2);
    ft_putstr_fd(": ", 2);
    ft_putendl_fd(err_msg, 2);
    return (127);
}

/*
** void exec_cmd(t_exec *exec)
** the function is executing the command if it's not built in.
** WIFEXITED(status): child exited normally.
** WEXITSTATUS(status): return code when child exits.
*/

int exec_cmd(t_exec *exec)
{
	pid_t	c_pid;
	int		status;

	if ((c_pid = fork()) == -1)
		exec->code_ret = execve_failure("fork", strerror(errno));
	else if (c_pid == 0)
	{
		if (execve(exec->args[0], exec->args, exec->envp) == -1)
			exec->code_ret = execve_failure(exec->args[0], strerror(errno));
		exit(0);
	}
	else
	{
		if(wait(&status) > 0)
		{
		    if (WIFEXITED(status) && !WEXITSTATUS(status))
                exec->code_ret = 0;
            else if (WIFEXITED(status) && WEXITSTATUS(status))
                exec->code_ret = WEXITSTATUS(status);
		}
	}
	return (exec->code_ret);
}
