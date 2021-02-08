/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 14:24:33 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/08 19:46:40 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void        check_for_failed(char *strer)
{
    //free data and exit
    //TODO: check leak memory
    ft_putendl_fd(strer, 2);
    exit_function(1);
}

// void  ft_waitpid_s(t_exec *exec, int size, int signal)
// {
//     int i;

//     i = 0;
//     while (i < size)
//     {
//         waitpid(exec->pid_s[i], &exec->status, 0);
//         kill(exec->pid_s[i], signal);
//         i++;
//     }
//     ft_free_arr((void**)&(exec->pid_s));
// }

void pipe_execution(t_list *pipe_cmd_list, t_exec *exec)
{
    int tmp_in;
    int tmp_out;
    t_cmd   *tmp_cmd;
    int pipe_fd[2];
    int size;
    int fd_in;
    int fd_out;
    int i;
    int status;

    (void)exec;

    tmp_in = dup(0);// Save in/out
    tmp_out = dup(1);
    size = ft_lstsize(pipe_cmd_list);
    exec->pid_s = malloc(sizeof(pid_t) * (size + 1));
    fd_in = dup(tmp_in);//default in
    i = 0;
    while (pipe_cmd_list)
    {
        dup2(fd_in, 0);
        close(fd_in);
        if (pipe_cmd_list->next == NULL)
            fd_out = dup(tmp_out);//Default out
        else
        {
            pipe(pipe_fd);
            fd_out = pipe_fd[1];
            fd_in = pipe_fd[0];
        }
        dup2(fd_out, 1);
        close(fd_out);
        exec->c_pid = fork();
        if (exec->c_pid == 0)
        {
            tmp_cmd = (t_cmd*)pipe_cmd_list->content;
            fill_args(tmp_cmd->word_list, exec);
             if (check_if_built_in(exec->args[0]))
                built_ins_execution(exec);
            else
            {
                if (get_cmd_path(exec->args, exec->envp))
                    if (execve(exec->args[0], exec->args, exec->envp) < 0)
                        check_for_failed(strerror(errno));
            }
            exit_function(1);
        }
        else if (exec->c_pid == -1)
            check_for_failed(strerror(errno));
        exec->pid_s[i++] = exec->c_pid;
        pipe_cmd_list = pipe_cmd_list->next;
    }
    exec->pid_s[i] = 0;
    //Restore in/out
    dup2(tmp_in, 0);
    dup2(tmp_out, 1);
    close(tmp_in);
    close(tmp_out);
    i = 0;
    while(i < size)
    {
        waitpid(exec->pid_s[i], &status, 0);
        kill(exec->pid_s[i], SIGPIPE);
        i++;
    }
}
// void pipe_execution(t_list *pipe_cmd_list, char **envp, t_exec *exec)
// {
//     int fd[2];
//     int tmp_fd;
//     int tmp_out;
//     t_cmd *tmp__cmd;
//     int size;
//     int j = 0;

//     size = ft_lstsize(pipe_cmd_list);
//     exec->pid_s = malloc(sizeof(int) * (size + 1));
//     tmp_fd = dup(0);
//     tmp_out = dup(1);
//     while (pipe_cmd_list)
//     {
//         if (pipe_cmd_list->next)
//             if (pipe(fd) == -1)
//                 check_for_failed(strerror(errno));
//         exec->c_pid = fork();
//         if (exec->c_pid == 0)
//         {
//             tmp__cmd = (t_cmd *)pipe_cmd_list->content;
//             fill_args(tmp__cmd->word_list, exec);
//             if (dup2(tmp_fd, 0) == -1)
//                 check_for_failed(strerror(errno));
//             if (pipe_cmd_list->next)
//                 if (dup2(fd[1], 1) == -1)
//                     check_for_failed(strerror(errno));
//             if (close(fd[0]) == -1)
//                 check_for_failed(strerror(errno));
//             if (check_if_built_in(exec->args[0]))
//                 built_ins_execution(exec, &envp);
//             else
//             {
//                 if (get_cmd_path(exec->args, envp))
//                     if (execve(exec->args[0], exec->args, envp) < 0)
//                         check_for_failed(strerror(errno));
//             }
//             exit_function(0);
//         }
//         else if (exec->c_pid == -1)
//             check_for_failed(strerror(errno));
//         else
//         {
//         //    if (close(fd[0]) == -1)
//         //         perror("close_fd_0");//check_for_failed(strerror(errno));
//             // if (close(fd[1]) == -1)
//                 // perror("close_fd_1");//check_for_failed(strerror(errno));
//             // tmp_fd = fd[0];
//         }
//         exec->pid_s[j++] = exec->c_pid;
//         pipe_cmd_list = pipe_cmd_list->next;
//     }
//     exec->pid_s[j] = 0;
//     // close(fd[0]);
//     // close(fd[1]);
//     dup2(tmp_fd, 0);
//     dup2(tmp_out, 1);
//     close(tmp_fd);
//     close(tmp_out);
//     ft_waitpid_s(exec, size, SIGPIPE);
// }

// cat | ls | cat