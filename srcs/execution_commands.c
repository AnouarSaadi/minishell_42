/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_commands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:00:04 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/02 12:08:44 by asaadi           ###   ########.fr       */
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
		kill(_pid, SIGPIPE);
	}
	else
	{
		if (execve(args[0], args, envp) == -1)
		{
			ft_putendl_fd(strerror(errno), 2);
			exit_function(1);
		}
	}
}

void non_built_ins_execution(t_exec *exec, char **envp)
{
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

void get_arr_of_args(t_cmd *cmd, t_exec *exec)
{
	if (!(exec->args = (char **)malloc(sizeof(char *) * (ft_lstsize(cmd->word_list) + 1))))
		ft_putendl_fd("ERROR AT MALLOC", 2);
	exec->index = 0;
	while (cmd->word_list != NULL)
	{
		exec->args[exec->index] = ft_strdup(cmd->word_list->content);
		cmd->word_list = cmd->word_list->next;
		exec->index++;
	}
	// cmd = cmd->next;
	exec->args[exec->index] = NULL;
}

int lst_cmd(t_list *tmp)
{
	t_list *tmps = tmp;
	int i = 0;
	while (tmp && ((t_token *)tmp->content)->type == e_state_nsc)
	{
		i++;
		tmp = tmp->next;
	}
	tmp = tmps;
	return (0);
}

void execution_cmds(t_list *token_list, char **envp)
{
	t_list *tmp;
	// t_cmd *cmd;
	t_exec *exec;
	t_token *token;
	int i;

	(void)envp;
	i = 0;
	exec = malloc(sizeof(t_exec));
	tmp = token_list;
	token = (t_token *)tmp->content;
	while (tmp)
	{
		i = 0;
		exec->args = malloc(sizeof(char*)*(lst_cmd(tmp) + 1));
		while (tmp && ((t_token *)tmp->content)->type == e_state_nsc) //((t_token *)tmp->content)
		{
			// puts(((t_token *)tmp->content)->value);
			exec->args[i] = ft_strdup(((t_token*)tmp->content)->value);
			i++;
			tmp = tmp->next;
		}
		exec->args[i] = NULL;
		// puts("##################");
		if (check_if_built_in(exec))
			built_ins_execution(exec, &envp);
		else
			non_built_ins_execution(exec, envp);
		// int s = 0;
		// fill_cmd(tmp, &cmd);
		// while (cmd && s < ft_lstsize(cmd->word_list))
		// {
		// 	// ft_putnbr_fd(ft_lstsize(cmd->word_list), 1);
		// 	// ft_putchar_fd('\n', 1);
		// 	get_arr_of_args(cmd, &exec);
		// 	// if (!(exec.args = (char **)malloc(sizeof(char*) * (ft_lstsize(cmd->word_list) + 1))))
		// 	// 	ft_putendl_fd("ERROR AT MALLOC", 2);
		// 	// exec.index = 0;
		// 	// if (cmd->word_list != NULL)
		// 	// {
		// 	// 	exec.args[exec.index] = ft_strdup(cmd->word_list->content);
		// 	// 	cmd->word_list = cmd->word_list->next;
		// 	// 	puts(exec.args[exec.index]);
		// 	// 	exec.index++;
		// 	// 	// puts("========list___001");
		// 	// 	// puts("heere");
		// 	// }
		// 	// exec.args[exec.index] = NULL;
		// 	cmd = cmd->next;
		// 	i = 0;
		// 	while (exec.args[i])
		// 	{
		// 		puts("========list final of words");
		// 		puts(exec.args[i]);
		// 		i++;
		// 		puts("=====================");
		// 		ft_putnbr_fd(i, 1);
		// 		ft_putchar_fd('\n', 1);
		// 		puts("=====================");
		// 	}
		// 	// if (ft_lstsize(cmd->word_list) == 0)
		// 	// 	break;

		// 	// ft_free_2dem_arr(exec.args);
		// 	s++;
		// }
		if (!tmp)
			break;
		tmp = tmp->next;
	}
}