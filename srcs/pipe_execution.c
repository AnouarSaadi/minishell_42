/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 14:24:33 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/07 13:00:28 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void pipe_execution(t_list *pipe_cmd_list, char **envp, t_exec *exec)
{
    int *fd = malloc(sizeof(int) * 2);
    int tmp_fd;
    int tmp_out;
    t_cmd *tmp__cmd;
    pid_t c_pid;
    int size;
    // int status;
    int i;
    int j = 0;

    size = ft_lstsize(pipe_cmd_list);
    int *pid = malloc(sizeof(int) * (size + 1));
    while (pipe_cmd_list)
    {
        // Save stdin and stdout
        tmp_fd = dup(0); // Origin tmp_fd = 0
        tmp_out = dup(1);
        if (pipe_cmd_list->next)
            pipe(fd); // check failure of pipe
        printf("|fd[0] = %d fd[1] == %d|\n", fd[0], fd[1]);
        c_pid = fork();
        if (c_pid == 0)
        {
            tmp__cmd = (t_cmd *)pipe_cmd_list->content;
            fill_args_from_list_words(tmp__cmd->word_list, exec);
            if (dup2(tmp_fd, 0) == -1) // check failure of pipe
                perror("dup1");
            if (pipe_cmd_list->next)
                if (dup2(fd[1], 1) == -1) // check failure of pipe
                    perror("dup2");
            if (close(fd[0]) == -1)
                perror("Close fd[0]");
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
        else if (c_pid < 0)
        {
            perror("fork");
            exit_function(1);
        }
        else
        {
           if (close(fd[0]) == -1)
            perror("2 - Close fd[0]");
            if (close(fd[1]) == -1)
                perror("Close fd[1]");
            tmp_fd = fd[0];
            // close(tmp_out);
        }
        pid[j] = c_pid;
        j++;
        pipe_cmd_list = pipe_cmd_list->next;
        // close(tmp_fd);
    }
    // close(tmp_fd); // ADD to Code
    // close(tmp_out);
    // Backup stdin and stdout
    dup2(tmp_fd, 0); // ADD to Code
    dup2(tmp_out, 1);
    i = 0;
    printf("%d\n", size);
    while (i < size)
    {
        // wait(0);
        waitpid(pid[i], &status, 0);
        kill(pid[i], SIGPIPE);
        i++;
    }
}

// cat | cat | ls