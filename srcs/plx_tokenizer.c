/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 10:36:01 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/04 12:33:00 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

/*
** first if: if current character is special character and the character before
** is escape character then this current character is no longer special 
** character and tok.state must be change to non speciat character
** second if : to stop tokens like | from geting appended and 
** (str="||" -> '|' '|')
** third if: to handletokens with sc tokens with double character 
** like '>>' or '<<'
*/

void	force_state(char *str, t_tok *tok)
{
	if (get_state(str[tok->i]) != e_state_nsc 
			&& check_token_type(ft_lstlast(tok->tokens), e_state_escape) == 1 
			&& is_between_quotes(tok->tokens) == 0)
	{
		tok->state = e_state_nsc;
	}
	else if (get_state(str[tok->i]) != e_state_nsc 
			&& get_state(str[tok->i]) != e_state_wspace && str[tok->i] != '>')
	{
		tok->state = e_state_delim;
	}
	else if (tok->j - 1 >= 0 && get_state(tok->start[tok->j - 1]) == tok->state 
			&& tok->state != e_state_nsc && tok->state != e_state_wspace)
	{
		tok->state = get_dstate(tok->start[tok->j]);
	}

}

/*
** if (tok->state == e_state_delim) this condition is to get back the real 
** tok.state because e_state_delim just to force change of tok.state and cut
*/

void	add_tokens(char *str, t_tok *tok)
{
	if (tok->state != get_state(str[tok->i]))
	{
		if (tok->state == e_state_delim)
			tok->state = get_state(tok->start[0]);
		tok->end = tok->start + tok->j;
		tok->t = (char *)malloc(sizeof(char) * (tok->end - tok->start + 1));
		ft_strlcpy(tok->t, tok->start, tok->end - tok->start + 1);
		ft_lstadd_back(&(tok->tokens), 
				ft_lstnew(create_token(tok->t, tok->state)));
		tok->state = get_state(str[tok->i]);
		tok->start = tok->end;
		tok->j = 0;
	}

}
t_list *ft_tokenizer(char *str)
{
	t_tok tok;

	tok.tokens = NULL;
	tok.start = str;
	tok.state = get_state(str[0]);
	tok.i = 0;
	tok.j = 0;
	while (str[tok.i] != '\0')
	{

		add_tokens(str, &tok);
		force_state(str, &tok);
		tok.i++;
		tok.j++;
	}
	if (*(tok.start) != '\0')
	{
		if (tok.state == e_state_delim)
			tok.state = get_state(tok.start[0]);
		ft_lstadd_back(&(tok.tokens), 
				ft_lstnew(create_token(ft_strdup(tok.start), tok.state)));
	}
	return (tok.tokens);
}

void free_token(t_list *elem)
{
	t_token *token;

	token = (t_token *)(elem->content);
	free(token->value);
	free(token);
	free(elem);
}


int is_redir(enum e_state type)
{
	if (type == e_state_gt || type == e_state_lt || type == e_state_dgt)
		return (1);
	return (0);
}


/*
** replace every duplicated char c in str to one e.i ***** -> *
*/ 

char *change_to_one(char *str, char c)
{
	char *res;
	int i;
	int j;

	res = (char *)malloc(sizeof(char) * ft_strlen(str) + 1);
	ft_bzero(res, ft_strlen(str) + 1);
	i = 0;
	j = 0;
	while ((size_t)i < ft_strlen(str))
	{
		res[j] = str[i];
		j++;
		if (str[i] == c)
			while (str[i] == c)
				i++;
		else
			i++;
	}
	return (res);
}

void create_pattern(t_list *tl)
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

void subs_wildcard(t_list *tl)
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

void wildcard(t_list **tl)
{
	t_list *tmp;
	t_list *head;

	//there was a problem when the pattern is the first elem of the list
	//what i did is to add another elem at the beginning of the list(head)
	//and then remove that elem
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
				break;
		}
		tmp = tmp->next;
	}
	*tl = head->next;
	free_token(head);
}

void switch_state(t_list *tl, enum e_state from, enum e_state to)
{
	while (tl != NULL)
	{
		if (((t_token *)tl->content)->type == from)
			((t_token *)tl->content)->type = to;
		tl = tl->next;
	}
}
