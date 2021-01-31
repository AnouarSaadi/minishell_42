/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_commands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:00:04 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/31 18:01:14 by asaadi           ###   ########.fr       */
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

void do_if_is_not_built_in(char **args, char **envp)
{
	if (get_cmd_path(args, envp))
		exec_cmd(args, envp);
	ft_free_2dem_arr(args);
}

void check_if_built_in(char **args, char ***envp, int *i)
{
	*i = 0;
	if (!ft_strcmp(args[0], "cd"))
		change_directory(args[1], *envp, i);
	if (!ft_strcmp(args[0], "pwd") ||
		!ft_strcmp(args[0], "PWD"))
	{
		pwd_function();
		*i = 1;
	}
	if (!ft_strcmp(args[0], "echo") ||
		!ft_strcmp(args[0], "ECHO"))
	{
		if (args[1] && !ft_strcmp(args[1], "-n"))
			echo_function(args, 1, i);
		else
			echo_function(args, 0, i);
	}
	if (!ft_strcmp(args[0], "export"))
	{
		if (args[1])
			export_function(envp, args, i);
		else
			sort_print_envp_alpha(*envp, i);
	}
	if (!ft_strcmp(args[0], "unset"))
		unset_function(envp, args, i);
	if (!ft_strcmp(args[0], "env") ||
		!ft_strcmp(args[0], "ENV"))
		env_function(*envp, i);
	if (!ft_strcmp(args[0], "exit"))
	{
		if (!args[1])
			exit_function(0);
		else
			exit_function(ft_atoi(args[1]));
		*i = 1;
	}
}

void execution_cmds(t_list *token_list, char **envp)
{
	t_list *tmp;
	t_token *token;

	(void)envp;
	tmp = token_list;
	while (tmp != NULL)
	{
		token = (t_token*)tmp->content;
		printf("|%s|:", token->value);
		if (token->type != e_state_nsc)
			printf("\e[1;32m sc: %d\n\e[0m", token->type);
		else
			printf("\e[1;32m nsc\n\e[0m");
		tmp = tmp->next;
	}
}