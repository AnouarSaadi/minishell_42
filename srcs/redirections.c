/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 12:04:12 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/21 11:18:13 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void dup2_fails(int err)
{
    ft_putstr_fd("bash: dup2: ", 2);
    ft_putendl_fd(strerror(err), 2);
    exit_func(1);
}

void redir_is_in_cmd(t_exec *exec, t_cmd *cmd)
{
    t_list *tmp__redir;
    int fds[2];
    int save_fds[2];

    save_fds[0] = dup(0);
    save_fds[1] = dup(1);
    tmp__redir = cmd->redir_list;
    while (tmp__redir)
    {
        if (((t_redir *)tmp__redir->content)->type == e_state_gt)
        {
            if ((fds[1] = open(((t_redir *)tmp__redir->content)->file, O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1)
            {
                ft_putstr_fd("bash: ", 2);
                ft_putstr_fd(((t_redir *)tmp__redir->content)->file, 2);
                ft_putstr_fd(": ", 2);
                ft_putendl_fd(strerror(errno), 2);
                exit(1);
            }
            if (dup2(fds[1], 1) == -1)
                dup2_fails(errno);
            close(fds[1]);
        }
        else if (((t_redir *)tmp__redir->content)->type == e_state_dgt)
        {
            if ((fds[1] = open(((t_redir *)tmp__redir->content)->file, O_CREAT | O_RDWR | O_APPEND, 0666)) == -1)
            {
                ft_putstr_fd("bash: ", 2);
                ft_putstr_fd(((t_redir *)tmp__redir->content)->file, 2);
                ft_putstr_fd(": ", 2);
                ft_putendl_fd(strerror(errno), 2);
                exit(1);
            }
            if (dup2(fds[1], 1) == -1)
                dup2_fails(errno);
            close(fds[1]);
        }
        else if (((t_redir *)tmp__redir->content)->type == e_state_lt)
        {
            if ((fds[0] = open(((t_redir *)tmp__redir->content)->file, O_RDONLY)) == -1)
            {
                ft_putstr_fd("bash: ", 2);
                ft_putstr_fd(((t_redir *)tmp__redir->content)->file, 2);
                ft_putstr_fd(": ", 2);
                ft_putendl_fd(strerror(errno), 2);
                exit(1);
            }
            if (dup2(fds[0], 0) == -1)
                dup2_fails(errno);
            close(fds[0]);
        }
        tmp__redir = tmp__redir->next;
    }
    exec->args = fill_args(cmd->word_list);
    if (check_if_built_in(exec->args[0]))
        built_ins_execution(exec);
    else
    {
        if (get_cmd_binary_path(exec))
            exec_cmd(exec);
    }
    if (dup2(save_fds[0], 0) == -1)
        dup2_fails(errno);
    close(save_fds[0]);
    if (dup2(save_fds[1], 1) == -1)
        dup2_fails(errno);
    close(save_fds[0]);
}