/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_syntax.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 15:12:50 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/06 12:53:24 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

t_list		*duplicate_tl(t_list *tl)
{
	t_list			*tmp;
	t_list			*res;
	enum e_state	type;

	res = NULL;
	tmp = tl;
	while (tmp != NULL)
	{
		type = ((t_token*)tmp->content)->type;
		if (type != e_state_wspace)
			ft_lstadd_back(&res, ft_lstnew(create_token(
							ft_strdup(((t_token*)tmp->content)->value), type)));
		tmp = tmp->next;
	}
	tmp = res;
	while (tmp != NULL)
	{
		type = ((t_token*)tmp->content)->type;
		if (type == e_state_wildcard || type == e_state_afterdollarqsm
				|| type == e_state_afterdollar)
			((t_token*)tmp->content)->type = e_state_nsc;
		tmp = tmp->next;
	}
	return (res);
}

t_list		*syntax_cmd(t_list *tl, int *error)
{
	enum e_state type;

	if (tl != NULL && (((t_token*)tl->content)->type != e_state_nsc
		&& is_redir(((t_token*)tl->content)->type) != 1))
	{
		*error = 1;
		return (tl);
	}
	while (tl != NULL)
	{
		type = ((t_token*)tl->content)->type;
		if (type == e_state_escape || (is_redir(type) == 1 && (tl->next == NULL
					|| ((t_token*)tl->next->content)->type != e_state_nsc)))
		{
			*error = 1;
			if (type != e_state_escape)
				return (tl->next);
			else
				return (tl);
		}
		if (type != e_state_nsc && is_redir(type) != 1)
			break ;
		tl = tl->next;
	}
	return (tl);
}

t_list		*syntax_pipe(t_list *tl, int *error)
{
	tl = syntax_cmd(tl, error);
	if (*error == 1)
		return (tl);
	while (tl != NULL && ((t_token*)tl->content)->type == e_state_pipe)
	{
		tl = syntax_cmd(tl->next, error);
		if (*error == 1)
			return (tl);
	}
	return (tl);
}

int			syntax_list(t_list *tl, int *error)
{
	t_list *tmp;

	if (*error == 2)
	{
		unexp_token("", *error);
		return (*error);
	}
	tl = duplicate_tl(tl);
	tmp = tl;
	tl = syntax_pipe(tl, error);
	if (*error == 1)
		unexp_token((tl != NULL)
				? ((t_token*)tl->content)->value : NULL, *error);
	while (*error != 1
			&& tl != NULL && ((t_token*)tl->content)->type == e_state_scolon)
	{
		tl = syntax_pipe(tl->next, error);
		if (*error == 1)
			unexp_token((tl != NULL)
				? ((t_token*)tl->content)->value : NULL, *error);
	}
	free_tokens_list(tmp);
	return (*error);
}

void		unexp_token(char *value, int error)
{
	if (error == 2)
	{
		ft_putstr_fd("minishell: syntax error", 2);
		ft_putstr_fd("\n", 2);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		if (value != NULL)
			ft_putstr_fd(value, 2);
		else
			ft_putstr_fd("newline", 2);
		ft_putstr_fd("'\n", 2);
	}
}
