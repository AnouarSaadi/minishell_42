/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 12:04:12 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/29 12:58:45 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void if_redir_is_in_cmd(char **args, t_cmd *cmd, char ***envp)
{
    while (cmd->redir_list)
    {
        if (((t_redir *)cmd->redir_list->content)->type == e_state_gt)
            redirect_to_std_out(args, ((t_redir *)cmd->redir_list->content)->file, envp);
        cmd->redir_list = cmd->redir_list->next;
    }
}