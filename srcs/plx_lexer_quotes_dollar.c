/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_lexer_quotes_dollar.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/04 11:45:05 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/06 12:37:38 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

/*
** Mr.Bricolage if quotes are succesive like '' add empty string between them
*/

enum e_state	subs_quotes(t_list *tl, enum e_state quote, enum e_state type)
{
	t_list *new_token;

	if (quote == 0 && (type == e_state_squote || type == e_state_dquote))
	{
		if (tl->next != NULL
				&& ((t_token*)tl->next->content)->type == type)
		{
			new_token = ft_lstnew(create_token(ft_strdup(""), e_state_nsc));
			ft_lstadd_front(&tl->next, new_token);
			tl->next = new_token;
		}
		quote = type;
	}
	else if (quote != 0 && type == quote)
		quote = 0;
	else if (quote != 0 && (quote != e_state_dquote ||
				((type != e_state_dollar || is_valid_after_dollar(tl) == 0)
		&& type != e_state_escape && type != e_state_escape)))
		((t_token*)tl->content)->type = e_state_nsc;
	return (quote);
}

void			quotes(t_list *tokens_list, int *error)
{
	enum e_state quote;
	enum e_state type;

	quote = 0;
	while (tokens_list != NULL)
	{
		type = ((t_token *)tokens_list->content)->type;
		quote = subs_quotes(tokens_list, quote, type);
		tokens_list = tokens_list->next;
	}
	if (quote != 0)
		*error = 2;
}

void			subs_dollar(t_list *tl)
{
	enum e_state type;

	if (tl->next != NULL)
	{
		type = ((t_token *)tl->next->content)->type;
		if (type == e_state_nsc || type == e_state_wildcard
				|| type == e_state_qsm)
		{
			if (type != e_state_qsm)
				((t_token *)tl->next->content)->type = e_state_afterdollar;
			else
				((t_token *)tl->next->content)->type = e_state_afterdollarqsm;
		}
	}
	if (tl->next == NULL || (type != e_state_squote && type != e_state_dquote
				&& type != e_state_nsc && type != e_state_wildcard
				&& type != e_state_qsm))
		((t_token *)tl->content)->type = e_state_nsc;
}

void			dollar(t_list *tl)
{
	while (tl != NULL)
	{
		if (((t_token *)tl->content)->type == e_state_dollar)
		{
			subs_dollar(tl);
		}
		tl = tl->next;
	}
}

t_list			*replace_afterdollar(t_list **tl, t_exec *exec)
{
	char			*env_name;
	enum e_state	type;
	t_list			*tmp;

	tmp = *tl;
	while (tmp != NULL && ((t_token *)tmp->content)->type != e_state_scolon)
	{
		type = ((t_token *)tmp->content)->type;
		if (type == e_state_afterdollar || type == e_state_afterdollarqsm)
		{
			env_name = ((t_token *)tmp->content)->value;
			if (type == e_state_afterdollar)
				((t_token *)tmp->content)->value =
					get_var_env(exec->envp, env_name);
			else
				((t_token *)tmp->content)->value = ft_itoa(exec->code_ret);
			((t_token *)tmp->content)->type = e_state_nsc;
			free(env_name);
		}
		tmp = tmp->next;
	}
	join_same_type(*tl, e_state_nsc, e_state_scolon);
	wildcard(tl);
	return (remove_token_by_type(tl, e_state_wspace, e_state_scolon));
}
