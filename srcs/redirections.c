/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 12:04:12 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/29 14:59:43 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void if_redir_is_in_cmd(char **args, t_cmd *cmd, char ***envp)
{
    while (cmd->redir_list)
    {
        if (((t_redir *)cmd->redir_list->content)->type == e_state_gt)
        {
            ft_putendl_fd("hawhaw", 1);
            redirect_to_std_out(args, ((t_redir *)cmd->redir_list->content)->file, envp);
        }
        else if (((t_redir *)cmd->redir_list->content)->type == e_state_dgt)
        {
            ft_putendl_fd("redirection double great", 1);
        }
        else if (((t_redir *)cmd->redir_list->content)->type == e_state_lt)
            redirect_to_std_in(args, ((t_redir*)cmd->redir_list->content)->file, envp);
        else if (((t_redir *)cmd->redir_list->content)->type == e_state_dlt)
        {
            ft_putendl_fd("redirection double less", 1);
        }
        cmd->redir_list = cmd->redir_list->next;
    }
}