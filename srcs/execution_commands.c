/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_commands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:00:04 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/02 17:48:48 by asaadi           ###   ########.fr       */
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
		if (execve(args[0], args, envp) == -1)
		{
			ft_putendl_fd(strerror(errno), 2);
			exit_function(1);
		}
	}
	wait(0);
}

void non_built_ins_execution(t_exec *exec, char **envp)
{
	puts("here");
	if (get_cmd_path(exec->args, envp))
		exec_cmd(exec->args, envp);
	// ft_free_2dem_arr(exec->args);
}

int check_if_built_in(t_exec *exec)
{
	if (!ft_strcmp(exec->args[0], "cd"))
		return (1);
	if (!ft_strcmp(exec->args[0], "pwd") ||
		!ft_strcmp(exec->args[0], "PWD"))
		return (1);
	if (!ft_strcmp(exec->args[0], "echo") ||
		!ft_strcmp(exec->args[0], "ECHO"))
		return (1);
	if (!ft_strcmp(exec->args[0], "export"))
		return (1);
	if (!ft_strcmp(exec->args[0], "unset"))
		return (1);
	if (!ft_strcmp(exec->args[0], "env") ||
		!ft_strcmp(exec->args[0], "ENV"))
		return (1);
	if (!ft_strcmp(exec->args[0], "exit"))
		return (1);
	return (0);
}

void built_ins_execution(t_exec *exec, char ***envp)
{
	if (!ft_strcmp(exec->args[0], "cd"))
		change_directory(exec->args[1], *envp);
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
			export_function(envp, exec->args);
		else
			sort_print_envp_alpha(*envp);
	}
	if (!ft_strcmp(exec->args[0], "unset"))
		unset_function(envp, exec->args);
	if (!ft_strcmp(exec->args[0], "env") ||
		!ft_strcmp(exec->args[0], "ENV"))
		env_function(*envp);
	if (!ft_strcmp(exec->args[0], "exit"))
	{
		if (!exec->args[1])
			exit_function(0);
		else
			exit_function(ft_atoi(exec->args[1]));
	}
}

// void get_arr_of_args(t_list *word_list, t_exec *exec)
// {
// 	if (!(exec->args = (char **)malloc(sizeof(char *) * (ft_lstsize(word_list) + 1))))
// 		ft_putendl_fd("ERROR AT MALLOC", 2);
// 	exec->index = 0;
// 	while (word_list != NULL)
// 	{
// 		exec->args[exec->index] = ft_strdup(word_list->content);
// 		word_list = word_list->next;
// 		exec->index++;
// 	}
// 	// cmd = cmd->next;
// 	exec->args[exec->index] = NULL;
// }

// int lst_cmd(t_list *tmp)
// {
// 	t_list *tmps = tmp;
// 	int i = 0;
// 	while (tmp && ((t_token *)tmp->content)->type == e_state_nsc)
// 	{
// 		i++;
// 		tmp = tmp->next;
// 	}
// 	tmp = tmps;
// 	return (0);
// }

void execution_cmds(t_list *token_list, char **envp)
{
	t_list *tmp__list;
	// t_cond *cond;
	// t_pipe *pipe;
	t_cmd *cmd;
	t_cmd *tmp__cmd;
	t_exec exec;
	int i;
	// t_pipe *pipe;

	tmp__list = token_list;
	// fill_cmd(tmp__list, cond);
	while (tmp__list != NULL)
	{
		// fill_cond(tmp__list, &cond);
		// if (cond)
		// fill_pipe(tmp__list, &pipe, cond->is_pipe);
		if (((t_token *)tmp__list->content)->type == e_state_nsc)
		{
			fill_cmd(tmp__list, &cmd);
			tmp__cmd = cmd;
			while (tmp__cmd != NULL)
			{
				if (!(exec.args = (char **)malloc(sizeof(char *) * (ft_lstsize(tmp__cmd->word_list) + 1))))
					ft_putendl_fd("ERROR AT MALLOC", 2);
				i = 0;
				while (tmp__cmd->word_list != NULL)
				{
					printf("arg0%d: |%s|\n", i, tmp__cmd->word_list->content);
					exec.args[i++] = ft_strdup(tmp__cmd->word_list->content);
					tmp__cmd->word_list = tmp__cmd->word_list->next;
					tmp__list = tmp__list->next;
				}
				exec.args[i] = NULL;
				// if (cmd)
				if (check_if_built_in(&exec))
					built_ins_execution(&exec, &envp);
				else
				{
					// non_built_ins_execution(&exec, envp);
				}
				// ft_free_2dem_arr(exec.args);
				tmp__cmd = tmp__cmd->next;
				
			}
		}
		// else if (tmp__list->next != NULL)÷
		else
			tmp__list = tmp__list->next;
			// break;
		printf("*********\n");
	}
}
// if (check_if_built_in(exec))
// 	built_ins_execution(exec, &envp);
// else
// 	non_built_ins_execution(exec, envp);