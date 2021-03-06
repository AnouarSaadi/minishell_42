/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_wildcard_path_list.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 14:56:08 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/05 16:03:16 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

t_list	*duplicate(t_list *path_tokens, char *dir_name)
{
	t_list	*tmp;
	t_list	*res;

	tmp = path_tokens;
	res = NULL;
	while (tmp != NULL)
	{
		if (((t_token*)tmp->content)->type == e_path_array)
			ft_lstadd_back(&res, ft_lstnew(create_token(
							ft_strdup(dir_name), (enum e_state)e_path_path)));
		else
			ft_lstadd_back(&res, ft_lstnew(create_token(
							ft_strdup(((t_token*)tmp->content)->value),
							(enum e_state)((t_token*)tmp->content)->type)));
		tmp = tmp->next;
	}
	return (res);
}

t_list	*get_path_list(t_list *path_tokens, char **dir_arr)
{
	int		i;
	t_list	*path_list;
	t_list	*dup;

	i = 0;
	path_list = NULL;
	sort_dir_arr(dir_arr);
	while (dir_arr[i] != NULL)
	{
		dup = duplicate(path_tokens, dir_arr[i]);
		join_same_type(dup, (enum e_state)e_path_path, 0);
		ft_lstadd_back(&path_list, ft_lstnew(dup));
		i++;
	}
	return (path_list);
}
