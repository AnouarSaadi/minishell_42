/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_tokenizer_utils_next.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 16:37:48 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/05 16:38:27 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

void	free_token(t_list *elem)
{
	t_token *token;

	token = (t_token *)(elem->content);
	free(token->value);
	free(token);
	free(elem);
}

int		is_redir(enum e_state type)
{
	if (type == e_state_gt || type == e_state_lt || type == e_state_dgt)
		return (1);
	return (0);
}

void	switch_state(t_list *tl, enum e_state from, enum e_state to)
{
	while (tl != NULL)
	{
		if (((t_token *)tl->content)->type == from)
			((t_token *)tl->content)->type = to;
		tl = tl->next;
	}
}
