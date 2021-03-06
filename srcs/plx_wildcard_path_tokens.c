/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_wildcard_path_tokens.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 14:28:52 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/05 16:10:38 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

/*
** this will set_type of token with correspandante one
*/

t_list	*set_type(t_list *tokens)
{
	t_list *tmp;

	tmp = tokens;
	while (tmp != NULL)
	{
		if (ft_strchr(((t_token*)tmp->content)->value, '*') != NULL)
			((t_token*)tmp->content)->type = (enum e_state)e_path_pattern;
		else
			((t_token*)tmp->content)->type = (enum e_state)e_path_path;
		tmp = tmp->next;
	}
	return (tokens);
}

void	push_token(t_list **tokens, char *start, char *end)
{
	char *t;

	t = (char*)malloc(sizeof(char) * (end - start + 1));
	ft_strlcpy(t, start, end - start + 1);
	ft_lstadd_back(tokens, ft_lstnew(create_token(t, 0)));
}

t_list	*split_path_tokens(char *str, t_list *tokens)
{
	int		i;
	char	*start;
	char	*end;
	int		change;

	start = str;
	change = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (change == 1)
		{
			change = 0;
			end = start + i;
			push_token(&tokens, start, end);
			start = end;
			i = 0;
		}
		if (start[i] == '/')
			change = 1;
		i++;
	}
	if (*start != '\0')
		ft_lstadd_back(&tokens, ft_lstnew(create_token(ft_strdup(start), 0)));
	return (set_type(tokens));
}

void	free_path_tokens(t_list *path_tokens)
{
	int		i;
	char	**arr;

	if (path_tokens != NULL && path_tokens->next != NULL)
		free_path_tokens(path_tokens->next);
	if (path_tokens != NULL
			&& ((t_token*)path_tokens->content)->type != e_path_array)
	{
		free_token(path_tokens);
	}
	else if (path_tokens != NULL)
	{
		arr = (char**)((t_token*)path_tokens->content)->value;
		i = 0;
		while (arr[i] != NULL)
		{
			free(arr[i]);
			i++;
		}
		free(arr);
		free((t_token*)path_tokens->content);
		free(path_tokens);
	}
}
