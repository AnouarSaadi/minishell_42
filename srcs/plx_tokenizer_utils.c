/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_tokenizer_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/04 11:11:54 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/04 11:20:12 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

enum e_state get_state(char c)
{
	if (c == ' ')
		return (e_state_wspace);
	else if (c == '&')
		return (e_state_and);
	else if (c == '|')
		return (e_state_pipe);
	else if (c == '$')
		return (e_state_dollar);
	else if (c == '\'')
		return (e_state_squote);
	else if (c == '"')
		return (e_state_dquote);
	else if (c == ';')
		return (e_state_scolon);
	else if (c == '>')
		return (e_state_gt);
	else if (c == '<')
		return (e_state_lt);
	else if (c == '\\')
		return (e_state_escape);
	else if (c == '*')
		return (e_state_wildcard);
	else if (c == '?')
		return (e_state_qsm);
	else
		return (e_state_nsc);
}

enum e_state get_dstate(char c)
{
	if (c == '>')
		return (e_state_dgt);
	return (e_state_delim);
}

t_token *create_token(char *value, enum e_state type)
{
	t_token *token;

	token = (t_token *)malloc(sizeof(t_token));
	token->value = value;
	token->type = type;
	return (token);
}

int check_token_type(t_list *elem, enum e_state type)
{
	if (elem != NULL && ((t_token *)elem->content)->type == type)
		return (1);
	return (0);
}

/*
** because if escape is between single quotes it no longer has 
** any effect on tokens.
** last if: if quotes are single quotes all sc become nsc
** else if dquote some of them are not switched
*/

int is_between_quotes(t_list *tl)
{
	enum e_state quote;
	enum e_state type;

	quote = 0;
	while (tl != NULL)
	{
		type = ((t_token *)tl->content)->type;
		if (quote == 0 && (type == e_state_squote || type == e_state_dquote))
		{
			quote = type;
		}
		else if (quote != 0 && quote == type)
		{
			quote = 0;
		}
		tl = tl->next;
	}
	if (quote != 0 && quote != e_state_dquote)
		return (1);
	else
		return (0);
}

