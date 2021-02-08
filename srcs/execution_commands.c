/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_commands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:00:04 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/08 19:25:55 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_cmd(char **args, char **envp)
{
	pid_t _pid;
	int status;

	_pid = 0;
	status = 0;
	_pid = fork();
	if (_pid == -1)
	{
		ft_putendl_fd(strerror(errno), 2);
		exit_function(1);
	}
	else if (_pid > 0)
	{
		waitpid(_pid, &status, 0);
		kill(_pid, SIGTERM);
	}
	else
	{
		if (execve(args[0], args, envp) < 0)
		{
			ft_putendl_fd(strerror(errno), 2);
			exit_function(1);
		}
	}
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

void built_ins_execution(t_exec *exec)
{
	if (!ft_strcmp(exec->args[0], "cd"))
		change_directory(exec->args[1], exec->envp);
	if (!ft_strcmp(exec->args[0], "pwd") ||
		!ft_strcmp(exec->args[0], "PWD"))
		pwd_function();
	if (!ft_strcmp(exec->args[0], "echo") ||
		!ft_strcmp(exec->args[0], "ECHO"))
	{
		if (exec->args[1] && !ft_strcmp(exec->args[1], "-n"))
			echo_function(exec->args, 1);
		else
			echo_function(exec->args, 0);
	}
	if (!ft_strcmp(exec->args[0], "export"))
	{
		if (exec->args[1])
			export_function(exec);
		else
			sort_print_envp_alpha(exec->envp);
	}
	if (!ft_strcmp(exec->args[0], "unset"))
		unset_function(exec);
	if (!ft_strcmp(exec->args[0], "env") ||
		!ft_strcmp(exec->args[0], "ENV"))
		env_function(exec->envp);
	if (!ft_strcmp(exec->args[0], "exit"))
	{
		if (!exec->args[1])
			exit_function(0);
		else
			exit_function(ft_atoi(exec->args[1]));
	}
}

void cmds_execution(t_exec *exec)
{
	if (check_if_built_in(exec->args[0]))
		built_ins_execution(exec);
	else
	{
		if (get_cmd_path(exec->args, exec->envp))
			exec_cmd(exec->args, exec->envp);
	}
}

void fill_args(t_list *list_words, t_exec *exec)
{
	int i;

	if (!(exec->args = (char **)malloc(sizeof(char *) * (ft_lstsize(list_words) + 1))))
		ft_putendl_fd("ERROR AT MALLOC", 2);
	i = 0;
	while (list_words != NULL)
	{
		exec->args[i++] = ft_strdup(list_words->content);
		list_words = list_words->next;
	}
	exec->args[i] = NULL;
}

void execution_cmds(t_list *token_list, t_exec *exec)
{
	t_list *tmp_list;
	t_pipe *pipe_list;
	t_cmd *tmp__cmd;
	// t_exec exec;

	tmp_list = token_list;
	while (tmp_list)
	{
		tmp_list = fill_pipe(tmp_list, &pipe_list, e_state_pipe);
		if (ft_lstsize(pipe_list->cmd_list) > 1)
			pipe_execution(pipe_list->cmd_list, exec);
		else
		{
			tmp__cmd = (t_cmd *)pipe_list->cmd_list->content;
			fill_args(tmp__cmd->word_list, exec);
			if (tmp__cmd->redir_list)
			{
				printf("Lst_redir_size%d\n", ft_lstsize(tmp__cmd->redir_list));
				redir_is_in_cmd(exec, tmp__cmd);
			}
			else
				cmds_execution(exec);
		}
		if (!tmp_list)
			break;
		tmp_list = tmp_list->next;
	}
}