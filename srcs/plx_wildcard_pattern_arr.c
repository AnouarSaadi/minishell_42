/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_wildcard_pattern_arr.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/05 14:41:23 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/05 16:57:25 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

void	append_slash(char **arr, char *onlydir)
{
	int		i;
	char	*tmp;

	if (onlydir == NULL)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		tmp = arr[i];
		arr[i] = ft_strjoin(arr[i], "/");
		free(tmp);
		i++;
	}
}

/*
** if the array is not empty replace pattern with the array
** else if it's empty(no match) keep it and change type to e_state_path
*/

void	replace_token(t_list *path_tokens, t_token *token, char *pattern,
		char *onlydir)
{
	(void)path_tokens;
	if (*((char**)token->value) != NULL)
	{
		append_slash((char**)token->value, onlydir);
		free(pattern);
		token->type = (enum e_state)e_path_array;
	}
	else
	{
		free((char**)token->value);
		token->value = pattern;
		(token)->type = (enum e_state)e_path_path;
	}
}

/*
** for the first elem if it's pattern
*/

void	pattern_to_array_first(t_list *path_tokens)
{
	char *pattern;
	char *onlydir;

	if (path_tokens != NULL
			&& ((t_token*)path_tokens->content)->type == e_path_pattern)
	{
		pattern = ((t_token*)path_tokens->content)->value;
		onlydir = ft_strchr(((t_token*)path_tokens->content)->value, '/');
		((t_token*)path_tokens->content)->value = (char*)
			get_dir_arr(".", onlydir,
					((t_token*)path_tokens->content)->value);
		replace_token(path_tokens, path_tokens->content, pattern, onlydir);
		join_same_type(path_tokens, (enum e_state)e_path_path, 0);
	}
}

/*
** get_dir_arr return char** and token->value takes char* so i cast it
** if onlydir join array elem with '/'
*/

void	pattern_to_array(t_list *path_tokens)
{
	t_list	*next;
	char	*pattern;
	char	*onlydir;
	t_list	*tmp;

	tmp = path_tokens;
	pattern_to_array_first(tmp);
	while (tmp != NULL)
	{
		if (((t_token*)tmp->content)->type == e_path_array)
			break ;
		next = tmp->next;
		if (next != NULL && ((t_token*)next->content)->type == e_path_pattern)
		{
			pattern = ((t_token*)next->content)->value;
			onlydir = ft_strchr(pattern, '/');
			((t_token*)next->content)->value = (char*)
				get_dir_arr(((t_token*)tmp->content)->value,
						onlydir, ((t_token*)next->content)->value);
			replace_token(next, next->content, pattern, onlydir);
			join_same_type(path_tokens, (enum e_state)e_path_path, 0);
			break ;
		}
		tmp = tmp->next;
	}
}
