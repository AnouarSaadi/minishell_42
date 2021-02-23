/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 12:04:12 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/23 12:54:19 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** fucntion print in the stderr the message of error if there's somthing wrong at the redirection work.
*/

void print_msg_at_fail(char *err_msg, char *name, t_exec *exec)
{
    ft_putstr_fd("bash: ", 2);
    ft_putstr_fd(name, 2);
    ft_putstr_fd(": ", 2);
    ft_putendl_fd(err_msg, 2);
    exec->index = 1;
    exec->code_ret = 1;
}

/*
** int ft_close_dup2_fds(int fd0, int fd1, t_exec *exec)
** function of close and dup the file descriptors and check the failure of dup2 and close
*/

int ft_close_dup2_fds(int fd0, int fd1, t_exec *exec)
{
    if (dup2(fd0, fd1) == -1)
        print_msg_at_fail(strerror(errno), "dup2", exec);
    if (close(fd0) == -1)
        print_msg_at_fail(strerror(errno), "close", exec);
    return (fd0);
}

/*
** void get_input_ouput(t_list *tmp__redir, t_exec *exec)
** function get the fds from redirections
*/

void get_input_ouput(t_list *tmp__redir, t_exec *exec)
{
    int fds[2];

    if (((t_redir *)tmp__redir->content)->type == e_state_gt)
    {
        if ((fds[1] = open(((t_redir *)tmp__redir->content)->file, O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1)
            print_msg_at_fail(strerror(errno), ((t_redir *)tmp__redir->content)->file, exec);
        else
            ft_close_dup2_fds(fds[1], 1, exec);
    }
    else if (((t_redir *)tmp__redir->content)->type == e_state_dgt)
    {
        if ((fds[1] = open(((t_redir *)tmp__redir->content)->file, O_CREAT | O_RDWR | O_APPEND, 0666)) == -1)
            print_msg_at_fail(strerror(errno), ((t_redir *)tmp__redir->content)->file, exec);
        else
            ft_close_dup2_fds(fds[1], 1, exec);
    }
    else if (((t_redir *)tmp__redir->content)->type == e_state_lt)
    {
        if ((fds[0] = open(((t_redir *)tmp__redir->content)->file, O_RDONLY)) == -1)
            print_msg_at_fail(strerror(errno), ((t_redir *)tmp__redir->content)->file, exec);
        else
            ft_close_dup2_fds(fds[0], 0, exec);
    }
}



int redir_is_in_cmd(t_exec *exec, t_cmd *cmd, int pipe)
{
    t_list *tmp__redir;
    int save_fds[2];

    save_fds[0] = dup(0);
    save_fds[1] = dup(1);
    exec->index = 0;
    tmp__redir = cmd->redir_list;
    while (tmp__redir)
    {
        get_input_ouput(tmp__redir, exec);
        tmp__redir = tmp__redir->next;
    }
    exec->args = fill_args(cmd->word_list);
    if (!exec->index)
    {
        if (check_if_built_in(exec->args[0]))
            exec->code_ret = built_ins_execution(exec);
        else
        {
            if (get_cmd_binary_path(exec) && !pipe)
                exec->code_ret =  exec_cmd(exec);
            else if (get_cmd_binary_path(exec) && pipe)
            {
                if (execve(exec->args[0], exec->args, exec->envp) == -1)
                {}
            }
        }
    }
    ft_free_2dem_arr((void***)&(exec->args));
    ft_close_dup2_fds(save_fds[0], 0, exec);
    ft_close_dup2_fds(save_fds[1], 1, exec);
    printf("ret at redir_is_in_cmd %d\n", exec->code_ret);
    return (exec->code_ret);
}