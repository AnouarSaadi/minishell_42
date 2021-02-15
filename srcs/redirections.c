/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 12:04:12 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/15 12:21:45 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		redir_is_in_cmd(t_exec *exec, t_cmd *cmd)
{
    t_list *tmp__redir;
    (void)exec;
    (void)cmd;
    int fd[2];

    fd[0] = dup(0);
    fd[1] = dup(1);
    // close(0);
    // close(1);
    tmp__redir = cmd->redir_list;
    while(tmp__redir)
    {
        if(((t_redir*)tmp__redir->content)->type == e_state_gt)
        {
            close(fd[1]);
            if((fd[1] = open(((t_redir*)tmp__redir->content)->file, O_CREAT | O_RDWR, 0666)) == -1)
            {
                ft_putstr_fd("bash: ", 2);
                ft_putstr_fd(((t_redir*)tmp__redir->content)->file, 2);
                ft_putstr_fd(": ", 2);
                ft_putendl_fd(strerror(errno), 2);
                exit(1);
            }
            dup2(fd[1], 1);
            close(fd[1]);
        }
        if(((t_redir*)tmp__redir->content)->type == e_state_lt)
        {
            close(fd[0]);
            if((fd[0] = open(((t_redir*)tmp__redir->content)->file, O_RDONLY)) == -1)
            {
                ft_putstr_fd("bash: ", 2);
                ft_putstr_fd(((t_redir*)tmp__redir->content)->file, 2);
                ft_putstr_fd(": ", 2);
                ft_putendl_fd(strerror(errno), 2);
                exit(1);
            }
            dup2(fd[0], 0);
            close(fd[0]);
        }
        tmp__redir = tmp__redir->next;
    }
    fill_args(cmd->word_list, exec);
    if (check_if_built_in(exec->args[0]))
        built_ins_execution(exec);
    else
    {
        if(get_cmd_path(exec))
            exec_cmd(exec);
    }
}