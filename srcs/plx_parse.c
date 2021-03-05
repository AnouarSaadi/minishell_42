/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 16:36:14 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/05 15:42:04 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

t_redir		*get_redir(t_list *tl)
{
	t_redir *redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	redir->type = ((t_token *)tl->content)->type;
	redir->file = ft_strdup(((t_token *)tl->next->content)->value);
	return (redir);
}

t_list		*fill_cmd(t_list *tl, t_cmd **cmd)
{
	enum e_state type;

	*cmd = (t_cmd *)malloc(sizeof(t_cmd));
	(*cmd)->word_list = NULL;
	(*cmd)->redir_list = NULL;
	while (tl != NULL)
	{
		type = ((t_token *)tl->content)->type;
		if (type != e_state_nsc && is_redir(type) == 0)
			break ;
		if (type == e_state_nsc)
		{
			ft_lstadd_back(&(*cmd)->word_list,
					ft_lstnew(ft_strdup(((t_token *)tl->content)->value)));
		}
		if (is_redir(type) == 1)
		{
			if (tl->next != NULL)
				ft_lstadd_back(&(*cmd)->redir_list, ft_lstnew(get_redir(tl)));
			tl = (tl->next != NULL) ? tl->next->next : NULL;
		}
		else
			tl = tl->next;
	}
	return (tl);
}

t_list		*fill_pipe(t_list *tokens_list, t_pipe **pipe)
{
	t_cmd *cmd;

	*pipe = (t_pipe *)malloc(sizeof(t_pipe));
	tokens_list = fill_cmd(tokens_list, &cmd);
	(*pipe)->cmd_list = ft_lstnew(cmd);
	while ((tokens_list != NULL)
			&& ((t_token *)tokens_list->content)->type == e_state_pipe)
	{
		tokens_list = fill_cmd(tokens_list->next, &cmd);
		ft_lstadd_back(&(*pipe)->cmd_list, ft_lstnew(cmd));
	}
	return (tokens_list);
}

t_list		*fill_list(t_list **tl, t_list **cond_list, t_exec *exec)
{
	t_pipe *pipe;
	t_list *tmp;

	tmp = replace_afterdollar(tl, exec);
	tmp = fill_pipe(tmp, &pipe);
	*cond_list = ft_lstnew(pipe);
	execution_part(pipe, exec, *tl, *cond_list);
	while (tmp != NULL && ((t_token *)tmp->content)->type == e_state_scolon)
	{
		if (tmp->next == NULL)
			tmp = NULL;
		else
		{
			tmp->next = replace_afterdollar(&(tmp->next), exec);
			tmp = fill_pipe(tmp->next, &pipe);
			execution_part(pipe, exec, *tl, *cond_list);
			ft_lstadd_back(cond_list, ft_lstnew(pipe));
		}
	}
	return (tmp);
}

void		parse(t_list **tokens_list, t_exec *exec, int *error)
{
	t_list *cond_list;

	if (syntax_list(*tokens_list, error) == 0 && *tokens_list != NULL)
	{
		fill_list(tokens_list, &cond_list, exec);
		free_tokens_list(*tokens_list);
		free_list(cond_list);
	}
	else if (*error != 0)
		exec->code_ret = 258;
}
