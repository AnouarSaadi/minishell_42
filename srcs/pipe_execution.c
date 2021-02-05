/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 14:24:33 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/05 17:48:26 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void pipe_execution(t_list *pipe_cmd_list, char **envp, t_exec *exec)
{
    int fd[2];
    int fdd;
    t_cmd *tmp__cmd;
    pid_t pid;
    int size;
	int status;
    int i;

    fdd = 0;
    size = ft_lstsize(pipe_cmd_list);
	//save fds
    while (pipe_cmd_list)
    {
        if (pipe_cmd_list->next)
            pipe(fd);
        pid = fork();
        if (pid == 0)
        {
            tmp__cmd = (t_cmd *)pipe_cmd_list->content;
            fill_args_from_list_words(tmp__cmd->word_list, exec);
            dup2(fdd, 0);
            if (pipe_cmd_list->next)
                dup2(fd[1], 1);
            close(fd[0]);
            // if(tmp__cmd->redir_list)
			// 	redir_is_in_cmd(exec, tmp__cmd, &envp);
            cmds_execution(exec, envp);
            exit_function(1);
        }
        else if (pid < 0)
        {
            waitpid(pid, &status, 0);
		    kill(pid, SIGTERM);
        }
        else
        {
            close(fd[0]);
            close(fd[1]);
            fdd = fd[0];
        }
        pipe_cmd_list = pipe_cmd_list->next;
    }
    close(fd[0]);
    i = 0;
    while(i < size)
    {
        wait(0);
        i++;
    }
}