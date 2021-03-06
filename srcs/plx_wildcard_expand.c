/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_wildcard_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 14:49:23 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/05 16:04:18 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

char	**get_arr(t_list *path_tokens)
{
	while (path_tokens != NULL)
	{
		if (((t_token*)path_tokens->content)->type == e_path_array)
			return ((char**)((t_token*)path_tokens->content)->value);
		path_tokens = path_tokens->next;
	}
	return (NULL);
}

void	replace_path_list(t_list **tmp, char **arr)
{
	t_list *to_remove;

	to_remove = *tmp;
	*tmp = get_path_list((*tmp)->content, arr);
	(ft_lstlast(*tmp))->next = to_remove->next;
	free_path_tokens(to_remove->content);
	free(to_remove);
}

void	expand_first(t_list **path_list)
{
	t_list	*tmp;
	char	**arr;

	tmp = *path_list;
	while (tmp != NULL && (ft_lstsize(tmp->content) > 1
				|| get_arr(tmp->content) != NULL))
	{
		pattern_to_array(tmp->content);
		arr = get_arr(tmp->content);
		if (arr != NULL)
			replace_path_list(&tmp, arr);
	}
	*path_list = tmp;
}

void	expand(t_list **path_list)
{
	t_list	*tmp;
	t_list	*tmp_path_tokens;
	char	**arr;

	expand_first(path_list);
	tmp = *path_list;
	while (tmp != NULL)
	{
		if (tmp->next != NULL && (ft_lstsize(tmp->next->content) > 1
					|| get_arr(tmp->next->content) != NULL))
		{
			tmp_path_tokens = tmp->next->content;
			pattern_to_array(tmp_path_tokens);
			arr = get_arr(tmp_path_tokens);
			if (arr != NULL)
				replace_path_list(&tmp->next, arr);
			tmp = *path_list;
		}
		else
			tmp = tmp->next;
	}
}
