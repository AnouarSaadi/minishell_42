/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_wildcard_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/04 15:24:34 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/05 15:26:17 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

void	create_pattern(t_list *tl)
{
	enum e_state type;
	enum e_state next_type;

	while (tl != NULL)
	{
		if (tl->next != NULL)
		{
			type = ((t_token *)tl->content)->type;
			next_type = ((t_token *)tl->next->content)->type;
			if ((type == e_state_wildcard && next_type == e_state_nsc)
					|| (type == e_state_nsc && next_type == e_state_wildcard))
			{
				((t_token *)tl->content)->type = e_state_wildcard;
				((t_token *)tl->next->content)->type = e_state_wildcard;
			}
		}
		tl = tl->next;
	}
}

void	subs_wildcard(t_list *tl)
{
	t_list *dir_list;
	t_list *wild_tmp;

	if ((dir_list =
			matched_dir_list(((t_token *)tl->next->content)->value)) != NULL)
	{
		wild_tmp = tl->next;
		tl->next = dir_list;
		(ft_lstlast(dir_list))->next = wild_tmp->next;
		free_token(wild_tmp);
	}
	else
		((t_token *)tl->next->content)->type = e_state_nsc;
}

/*
** there was a problem when the pattern is the first elem of the list
** what i did is to add another elem at the beginning of the list(head)
** and then remove that elem
*/

void	wildcard(t_list **tl)
{
	t_list *tmp;
	t_list *head;

	tmp = *tl;
	ft_lstadd_front(&tmp, ft_lstnew(create_token(ft_strdup("tmp"), 0)));
	head = tmp;
	while (tmp != NULL)
	{
		if (tmp->next != NULL)
		{
			if (((t_token *)tmp->next->content)->type == e_state_wildcard)
				subs_wildcard(tmp);
			else if (((t_token *)tmp->next->content)->type == e_state_scolon)
				break ;
		}
		tmp = tmp->next;
	}
	*tl = head->next;
	free_token(head);
}
