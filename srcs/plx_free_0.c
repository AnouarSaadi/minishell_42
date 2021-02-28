/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_free_0.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 15:16:16 by abel-mak          #+#    #+#             */
/*   Updated: 2021/02/28 15:30:54 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

void	free_tokens_list(t_list *tokens_list)
{
	if (tokens_list != NULL)
	{
		free_tokens_list(tokens_list->next);
		free_token(tokens_list);
	}
}

void	free_word_list(t_list *word_list)
{
	char *str;

	if (word_list != NULL)
	{
		free_word_list(word_list->next);
		str = (char*)word_list->content;
		free(str);
		free(word_list);
	}
}

void	free_redir_list(t_list *redir_list)
{
	char *file;
	t_redir *redir;

	if (redir_list)
	{
		free_redir_list(redir_list->next);
		redir = (t_redir*)redir_list->content;
		file = redir->file;
		free(file);
		free((t_redir*)redir);
		free((t_list*)redir_list);
		
	}
}

void	free_cmd_list(t_list *cmd_list)
{
	t_list	*word_list;
	t_list	*redir_list;
	t_cmd	*cmd;

	if (cmd_list != NULL)
	{
		free_cmd_list(cmd_list->next);
		cmd = (t_cmd*)cmd_list->content;
		word_list = cmd->word_list;
		redir_list = cmd->redir_list;
		free_word_list(word_list);
		free_redir_list(redir_list);
		free((t_cmd*)cmd);
		free((t_list*)cmd_list);
	}
}

void	free_list(t_list *cond_list)
{
	t_pipe	*pipe;

	if (cond_list != NULL)
	{
		free_list(cond_list->next);
		pipe = (t_pipe*)cond_list->content;
		free_cmd_list(pipe->cmd_list);
		free((t_pipe*)pipe);
		free(cond_list);
	}
}
