/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_commands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:00:04 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/21 18:08:20 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** void exec_cmd(t_exec *exec)
** the function is executing the command if it's not built in. 
*/

void exec_cmd(t_exec *exec)
{
	pid_t _pid;

	_pid = 0;
	_pid = fork();
	if (_pid == -1)
	{
		ft_putendl_fd(strerror(errno), 2);
		exit_func(exec);
	}
	else if (_pid == 0)
	{
		if (execve(exec->args[0], exec->args, exec->envp) == -1)
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(exec->args[0], 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			exec->code_ret = 1;
		}
		exit(127);
	}
	else
		waitpid(_pid, &exec->status, 0);
}

int check_if_built_in(char *cmd)
{
	if (cmd && ft_strcmp(cmd, "env") && ft_strcmp(cmd, "cd") &&
		ft_strcmp(cmd, "pwd") && ft_strcmp(cmd, "exit") &&
		ft_strcmp(cmd, "export") && ft_strcmp(cmd, "unset") &&
		ft_strcmp(cmd, "echo"))
		return (0);
	return (1);
}

/*
** void built_ins_execution(t_exec *exec)
** The function is used when the command it's bult in.
** fill the code_ret by the value of execute program.
*/

void built_ins_execution(t_exec *exec)
{
	if (exec->args[0] && !ft_strcmp(exec->args[0], "cd"))
		exec->code_ret = change_directory(exec->args[1], exec);
	else if (exec->args[0] && !ft_strcmp(exec->args[0], "pwd"))
		exec->code_ret = pwd_function();
	else if (exec->args[0] && !ft_strcmp(exec->args[0], "echo"))
		exec->code_ret = echo_function(exec->args);
	else if (exec->args[0] && !ft_strcmp(exec->args[0], "exit"))
		exec->code_ret = exit_func(exec);
	else if (exec->args[0] && !ft_strcmp(exec->args[0], "export"))
		exec->code_ret = export_function(exec);
	else if (exec->args[0] && !ft_strcmp(exec->args[0], "unset"))
		exec->code_ret = unset_function(exec);
	else if (exec->args[0] && !ft_strcmp(exec->args[0], "env"))
		exec->code_ret = env_function(exec->envp);
}

void cmds_execution(t_exec *exec)
{
	if (check_if_built_in(exec->args[0]))
		built_ins_execution(exec);
	else
	{
		if (get_cmd_binary_path(exec))
			exec_cmd(exec);
	}
}

char **fill_args(t_list *list_words)
{
	int i;
	char **args;

	if (!(args = (char **)malloc(sizeof(char *) * (ft_lstsize(list_words) + 1))))
		ft_putendl_fd("ERROR AT MALLOC", 2);
	i = 0;
	while (list_words != NULL)
	{
		args[i++] = ft_strdup(list_words->content);
		list_words = list_words->next;
	}
	args[i] = NULL;
	return (args);
}

void execution_cmds(t_list *token_list, t_exec *exec)
{
	t_list *tmp_list;
	t_pipe *pipe_list;
	t_cmd *tmp__cmd;

	exec->status = 0;
	tmp_list = token_list;
	while (tmp_list)
	{
		tmp_list = fill_pipe(tmp_list, &pipe_list, e_state_pipe);
		if (ft_lstsize(pipe_list->cmd_list) > 1)
			pipe_execution(pipe_list->cmd_list, exec);
		else
		{
			tmp__cmd = (t_cmd *)pipe_list->cmd_list->content;
			if (tmp__cmd->redir_list)
				redir_is_in_cmd(exec, tmp__cmd);
			else
			{
				exec->args = fill_args(tmp__cmd->word_list);
				cmds_execution(exec);
			}
		}
		if (!tmp_list)
			break;
		tmp_list = tmp_list->next;
	}
	// ft_putnbr_fd(exec->code_ret, 1);
	// ft_putchar_fd('\n', 1);
}