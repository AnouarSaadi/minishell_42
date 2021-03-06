/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 11:09:32 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/05 15:43:10 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

/*
** so every path_list elem will contain path_tokens as content.
** if path_list content(path_tokens) contain token type pattern it means
** that it should be expanded to another path_list and so on...
** whenever we reach state where path_list has no more content containing tokens
** type pattern it's done this one should not be expanded
**
** Procedure:
** step1: check if there is token type pattern
** step2: replace that token with e_path_array
** step3: move from e_path_array to e_path_path (duplicate)
*/

int		path_exist(char *pathname)
{
	struct stat s;

	if (stat(pathname, &s) == -1 && (errno == ENOENT || errno == EACCES))
		return (0);
	return (1);
}

t_list	*get_dir_list_tokens(t_list *path_list)
{
	t_list	*res;
	t_list	*tmp;
	char	*path;

	res = NULL;
	tmp = path_list;
	while (tmp != NULL)
	{
		path = ((t_token*)((t_list*)tmp->content)->content)->value;
		if (path_exist(path) == 1)
		{
			ft_lstadd_back(&res,
				ft_lstnew(create_token(change_to_one(path, '/'), e_state_nsc)));
		}
		tmp = tmp->next;
	}
	return (res);
}

void	free_path_list(t_list *path_list)
{
	if (path_list != NULL && path_list->next != NULL)
		free_path_list(path_list->next);
	if (path_list != NULL)
	{
		free_path_tokens(path_list->content);
		free(path_list);
	}
}

t_list	*matched_dir_list(char *pattern)
{
	t_list	*path_list;
	t_list	*path_tokens;
	t_list	*dir_list;
	char	*simplifyed_pattern;

	path_tokens = NULL;
	simplifyed_pattern = change_to_one(pattern, '*');
	path_tokens = split_path_tokens(simplifyed_pattern, path_tokens);
	join_same_type(path_tokens, (enum e_state)(e_path_path), 0);
	pattern_to_array(path_tokens);
	path_list = ft_lstnew(path_tokens);
	expand(&path_list);
	dir_list = get_dir_list_tokens(path_list);
	free_path_list(path_list);
	free(simplifyed_pattern);
	return (dir_list);
}
