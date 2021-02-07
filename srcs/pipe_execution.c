/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 14:24:33 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/07 16:11:06 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void        check_for_failed(char *strer)
{
    //free data and exit
    ft_putendl_fd(strer, 2);
    exit_function(1);
}

void pipe_execution(t_list *pipe_cmd_list, char **envp, t_exec *exec)
{
    int fd[2];
    int tmp_fd;
    int tmp_out;
    t_cmd *tmp__cmd;
    pid_t c_pid;
    int size;
    int status;
    int i;
    int j = 0;

    size = ft_lstsize(pipe_cmd_list);
    int *pid = malloc(sizeof(int) * (size + 1));
    while (pipe_cmd_list)
    {
        tmp_fd = dup(0);
        tmp_out = dup(1);
        if (pipe_cmd_list->next)
            if (pipe(fd) == -1)
                check_for_failed(strerror(errno));
        c_pid = fork();
        if (c_pid == 0)
        {
            tmp__cmd = (t_cmd *)pipe_cmd_list->content;
            fill_args_from_list_words(tmp__cmd->word_list, exec);
            if (dup2(tmp_fd, 0) == -1)
                check_for_failed(strerror(errno));
            if (pipe_cmd_list->next)
                if (dup2(fd[1], 1) == -1)
                    check_for_failed(strerror(errno));
            if (close(fd[0]) == -1)
                check_for_failed(strerror(errno));// check failure of pipe
                // perror("Close fd[0]");
            // if(tmp__cmd->redir_list)
            // 	redir_is_in_cmd(exec, tmp__cmd, &envp);
            if (check_if_built_in(exec->args[0]))
                built_ins_execution(exec, &envp);
            else
            {
                if (get_cmd_path(exec->args, envp))
                    if (execve(exec->args[0], exec->args, envp) < 0)
                    {
                        ft_putendl_fd(strerror(errno), 2);
                        exit_function(1);
                    }
            }
            exit_function(0);
        }
        else if (c_pid == -1)
        {
            check_for_failed(strerror(errno));
        }
        else
        {
           if (close(fd[0]) == -1)
                check_for_failed(strerror(errno));
            if (close(fd[1]) == -1)
                check_for_failed(strerror(errno));
            tmp_fd = fd[0];
        }
        pid[j++] = c_pid;
        // j++;
        pipe_cmd_list = pipe_cmd_list->next;
    }

    dup2(tmp_fd, 0);
    dup2(tmp_out, 1);
    close(tmp_fd);
    close(tmp_out);
    i = 0;
    printf("%d\n", size);
    while (i < size)
    {
        // wait(0);
        waitpid(pid[i], &status, 0);
        kill(pid[i], SIGPIPE);
        i++;
    }
    ft_free_arr((void**)&pid);
}

// cat | cat | ls