/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 14:24:33 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/16 18:27:41 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Pipe. Run command1 and command2 in parallel.
** command1’s standard output is hooked up to command2’s standard input.
** Thus, command2 reads what command1 wrote.
** The exit status of the pipeline is the exit status of command2.
*/

void check_for_failed(char *strer)
{
    //free data and exit
    //TODO: check leak memory
    ft_putendl_fd(strer, 2);
    exit_func(1);
}

void pipe_execution(t_list *pipe_cmd_list, t_exec *exec)
{
    int save_fds[2];
    t_cmd *tmp_cmd;
    int pipe_fd[2];
    int size;
    int fds[2];
    int i;
    int status;

    (void)exec;

    save_fds[0] = dup(0); // Save in/out
    save_fds[1] = dup(1);
    size = ft_lstsize(pipe_cmd_list);
    if(!(exec->pid_s = malloc(sizeof(pid_t) * (size + 1))))
        ft_putendl_fd("Allocation Error", 2);
    fds[0] = dup(save_fds[0]); //default in if there's no redirection to stdin
    i = 0;
    while (pipe_cmd_list)
    {
        //redirect input
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
        exec->c_pid = fork();
        if (exec->c_pid == 0)
        {
            close(pipe_fd[0]);
            tmp_cmd = (t_cmd *)pipe_cmd_list->content;
            if (tmp_cmd->redir_list)
                redir_is_in_cmd(exec, tmp_cmd);
            else
            {
                fill_args(tmp_cmd->word_list, exec);
                if (check_if_built_in(exec->args[0]))
                    built_ins_execution(exec);
                else
                {
                    if (get_cmd_path(exec))
                        if (execve(exec->args[0], exec->args, exec->envp) == -1)
                            check_for_failed(strerror(errno));
                }
            }
            exit_func(1);
        }
        else if (exec->c_pid == -1)
            check_for_failed(strerror(errno));
        exec->pid_s[i++] = exec->c_pid;
        pipe_cmd_list = pipe_cmd_list->next;
    }
    exec->pid_s[i] = 0;
    dup2(save_fds[0], 0);
    dup2(save_fds[1], 1);
    close(save_fds[0]);
    close(save_fds[1]);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    i = 0;
    while (i < size)
    {
        waitpid(exec->pid_s[i], &status, 0);
        if(WEXITSTATUS(status) && !exec->status)
            exec->status = WEXITSTATUS(status);
        // else if(!exec->status && WEXITSTATUS(status))
            // exec->status = 127;
        i++;
    }
}