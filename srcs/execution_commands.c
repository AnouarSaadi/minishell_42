/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_commands.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 11:00:04 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/26 11:09:51 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** int check_if_built_in(char *) 
** return 1 if the arg is a one of the builtins and 0 if not.
*/

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

int built_ins_execution(t_exec *exec)
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
	return (exec->code_ret);
}

/*
** void cmds_execution(t_exec *exec, int pipe)
** function of executing the commands, it check the command if builtin or not and work on it.
** pipe var is used if the command is one of list of pipe list. it use for check if i have to create child or not.
** in pipe function i create the child so i don't need to fork for the seconde time.
*/

void cmds_execution(t_exec *exec, int pipe)
{
	exec->code_ret = 0;
	if (check_if_built_in(exec->args[0]))
		exec->code_ret = built_ins_execution(exec);
	else
	{
		if (!pipe && get_cmd_binary_path(exec))
			exec->code_ret = exec_cmd(exec);
		else if (pipe && get_cmd_binary_path(exec))
            if (execve(exec->args[0], exec->args, exec->envp) == -1)
				exec->code_ret = execve_failure(exec->args[0], strerror(errno));
	}
}

/*
** char **fill_args(t_list *)
** take the list and store it to a table 2D
*/

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

/*
** void execution_part(t_list *token_list, t_exec *exec)
** the begining of execution.
*/

void execution_part(t_list *token_list, t_exec *exec)
{
	t_list *tmp_list;
	t_pipe *pipe_list;
	t_cmd *tmp__cmd;

	tmp_list = token_list;
	while (tmp_list)
	{
		tmp_list = fill_pipe(tmp_list, &pipe_list);
		if (ft_lstsize(pipe_list->cmd_list) > 1)
			pipe_execution(pipe_list->cmd_list, exec);
		else
		{
			tmp__cmd = (t_cmd *)pipe_list->cmd_list->content;
			if (tmp__cmd->redir_list)
				redir_is_in_cmd(exec, tmp__cmd, 0);
			else
			{
				exec->args = fill_args(tmp__cmd->word_list);
				cmds_execution(exec, 0);
			}
		}
		if (!tmp_list)
			break;
		tmp_list = tmp_list->next;
	}
}