/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 10:36:01 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/05 16:38:47 by abel-mak         ###   ########.fr       */
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

t_list	*ft_tokenizer(char *str)
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
