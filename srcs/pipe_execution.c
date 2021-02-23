/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 14:24:33 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/23 17:59:28 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** static int execution__cmd(t_list *pipe_cmd_list, t_exec *exec)
** function use to execute the pipe command
*/

static int execution__cmd(t_list *pipe_cmd_list, t_exec *exec)
{
    t_cmd *tmp_cmd;

    tmp_cmd = (t_cmd *)pipe_cmd_list->content;
    if (tmp_cmd->redir_list)
        exec->code_ret = redir_is_in_cmd(exec, tmp_cmd, 1);
    else
    {
        exec->args = fill_args(tmp_cmd->word_list);
        cmds_execution(exec, 1);
    }
    return (exec->code_ret);
}

/*
** void get_return_cmd__pipe(t_exec *exec, int size)
** function that check for each child created by fork of pipe and get status.
** then get the code_ret by each command execute in that child.
*/

void get_return_cmd__pipe(t_exec *exec, int size)
{
    int i;
    int status;

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

/*
** void pipe_execution(t_list *pipe_cmd_list, t_exec *exec) 
** save_fds 2 file descriptors use in save the default in/out
** pipe_fd 2 file descriptors that the pipe use them for create a her in/out
** fds 2 file descriptos use to get in/out from pipe
*/

void pipe_execution(t_list *pipe_cmd_list, t_exec *exec)
{
    int save_fds[2];
    int pipe_fd[2];
    int size;
    int fds[2];

    save_fds[0] = dup(0); // Save in/out
    save_fds[1] = dup(1);
    size = ft_lstsize(pipe_cmd_list);
    fds[0] = dup(save_fds[0]); //default in if there's no redirection to stdin
    while (pipe_cmd_list)
    {
        ft_close_dup2_fds(fds[0], 0, exec);
        if (pipe_cmd_list->next == NULL)
            fds[1] = dup(save_fds[1]); //Default out if there's no redirection to stdout
        else
        {
            pipe(pipe_fd);
            fds[1] = pipe_fd[1];
            fds[0] = pipe_fd[0];
        }
        ft_close_dup2_fds(fds[1], 1, exec);
        exec->code_ret = 0;
        exec->c_pid = fork();
        if (exec->c_pid == 0)
        {
            close(pipe_fd[0]);
            exit(execution__cmd(pipe_cmd_list, exec));
        }
        else if (exec->c_pid == -1)
            exec->code_ret = execve_failure("fork", strerror(errno));
        pipe_cmd_list = pipe_cmd_list->next;
    }
    ft_close_dup2_fds(save_fds[0], 0, exec);
    ft_close_dup2_fds(save_fds[1], 1, exec);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    get_return_cmd__pipe(exec, size);
}