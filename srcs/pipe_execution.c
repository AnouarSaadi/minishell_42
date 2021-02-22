/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 14:24:33 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/22 19:26:56 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int execve_failure(char *arg, char *err_msg)
{
    ft_putstr_fd("bash: ", 2);
    ft_putstr_fd(arg, 2);
    ft_putstr_fd(": ", 2);
    ft_putendl_fd(err_msg, 2);
    return (127);
}

static int execution__cmd(t_list *pipe_cmd_list, t_exec *exec)
{
    t_cmd *tmp_cmd;

    tmp_cmd = (t_cmd *)pipe_cmd_list->content;
    if (tmp_cmd->redir_list)
        redir_is_in_cmd(exec, tmp_cmd);
    else
    {
        exec->args = fill_args(tmp_cmd->word_list);
        if (check_if_built_in(exec->args[0]))
            exec->code_ret = built_ins_execution(exec);
        else
        {
            if (get_cmd_binary_path(exec))
            {
                if (execve(exec->args[0], exec->args, exec->envp) == -1)
                    exec->code_ret = execve_failure(exec->args[0], strerror(errno));
            }
        }
    }
    return (exec->code_ret);
}

void pipe_execution(t_list *pipe_cmd_list, t_exec *exec)
{
    // t_cmd *tmp_cmd;
    int save_fds[2];
    int pipe_fd[2];
    int size;
    int fds[2];
    int i;
    int status;
    
    save_fds[0] = dup(0); // Save in/out
    save_fds[1] = dup(1);
    size = ft_lstsize(pipe_cmd_list);
    fds[0] = dup(save_fds[0]); //default in if there's no redirection to stdin
    i = 0;
    while (pipe_cmd_list)
    {
        dup2(fds[0], 0);
        close(fds[0]);
        if (pipe_cmd_list->next == NULL)
            fds[1] = dup(save_fds[1]); //Default out if there's no redirection to stdout
        else
        {
            pipe(pipe_fd);
            fds[1] = pipe_fd[1];
            fds[0] = pipe_fd[0];
        }
        dup2(fds[1], 1);
        close(fds[1]);
        exec->code_ret = 0;
        exec->c_pid = fork();
        if (exec->c_pid == 0)
        {
            close(pipe_fd[0]);
            exit(execution__cmd(pipe_cmd_list, exec));
        }
        else if (exec->c_pid == -1)
        {
            /* code */
        }
        pipe_cmd_list = pipe_cmd_list->next;
    }
    dup2(save_fds[0], 0);
    dup2(save_fds[1], 1);
    close(save_fds[0]);
    close(save_fds[1]);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    i = -1;
    while (++i < size)
    {
        if (wait(&status) > 0)
        {
            if (WIFEXITED(status) && !WEXITSTATUS(status))
                exec->code_ret = 0;
            else if (WIFEXITED(status) && WEXITSTATUS(status))
                exec->code_ret = WEXITSTATUS(status);
        }
    }
}