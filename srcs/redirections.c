/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 12:04:12 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/07 16:22:59 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int redirect_to_std_in(char *name)
{
    int _fd;

    if ((_fd = open(name, O_RDONLY, S_IRUSR)) < 0)
        ft_putendl_fd(strerror(errno), 2);
    return (_fd);
}

int redirect_to_std_in_append(char *name)
{
    int _fd;

    if ((_fd = open(name, O_RDONLY, S_IRUSR)) < 0)
        ft_putendl_fd(strerror(errno), 2);
    return (_fd);
}

int redirect_to_std_out(char *name)
{
    int _fd;

    if ((_fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0777)) < 0)
        ft_putendl_fd(strerror(errno), 2);
    return (_fd);
}

void redir_is_in_cmd(t_exec *exec, t_cmd *cmd, char ***envp)
{
    int fd;
    (void)exec;
    (void)envp;
    pid_t c_pid;

    while (cmd->redir_list)
    {
        c_pid = fork(); //TODO: fork for each redirection 
        if (((t_redir *)cmd->redir_list->content)->type == e_state_gt)
        {
            if (fd)
                close(fd);
            fd = redirect_to_std_out(((t_redir *)cmd->redir_list->content)->file);
            dup2(fd, 1);
        }
        else if (((t_redir *)cmd->redir_list->content)->type == e_state_dgt)
        {
            ft_putendl_fd("redirection double great", 1);
        }
        else if (((t_redir *)cmd->redir_list->content)->type == e_state_lt)
        {
            if (fd)
                close(fd);
            fd = redirect_to_std_in(((t_redir *)cmd->redir_list->content)->file);
            dup2(fd, 0);
        }
        else if (((t_redir *)cmd->redir_list->content)->type == e_state_dlt)
        {
            ft_putendl_fd("redirection double less", 1);
        }
        if (check_if_built_in(exec->args[0]))
            built_ins_execution(exec, envp);
        else
        {
            if (get_cmd_path(exec->args, *envp))
                exec_cmd(exec->args, *envp);
        }
        cmd->redir_list = cmd->redir_list->next;
    }
    close(fd);
}