/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 09:09:54 by abel-mak          #+#    #+#             */
/*   Updated: 2020/12/26 12:00:31 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"

typedef struct	s_cmd
{
	char *name;
	t_list *arg_list;
	t_list *redir_list;
}				t_cmd;

enum	e_state
{
	e_state_nsc,
	e_state_sc,
	e_state_and,
	e_state_dand,
	e_state_pipe,
	e_state_dpipe,
	e_state_squote,
	e_state_dquote,
	e_state_scolon,
	e_state_gt,
	e_state_dgt,
	e_state_lt,
	e_state_dlt,
	e_state_openparen,
	e_state_closeparen,
	e_state_escape,
	e_state_delim,
	e_state_wspace
};

int is_sc(char c)
{
	char *sp;

	sp = "\"'|&;<>()$\\";
	while (*sp != '\0')
	{
		if (*sp == c)
			return (1);
		sp++;
	}
	return (0);
}

enum e_state	get_state(char c)
{
	if (c == ' ')
		return (e_state_wspace);
	else if (c == '&')
		return (e_state_and);
	else if (c == '|')
		return (e_state_pipe);
	else if (c == '\'')
		return (e_state_squote);
	else if (c == '"')
		return (e_state_dquote);
	else if (c == ';')
		return (e_state_scolon);
	else if (c == '>')
		return (e_state_gt);
	else if (c == '<')
		return (e_state_lt);
	else if (c == '(')
		return (e_state_openparen);
	else if (c == ')')
		return (e_state_closeparen);
	else if (c == '\\')
		return (e_state_escape);
//	else if (is_sc(c) == 1)
//		return (e_state_sc);
	else
		return (e_state_nsc);
}

enum e_state	get_dstate(char c)
{
	if (c == '&')
		return (e_state_dand);
	else if (c == '|')
		return (e_state_dpipe);
	else if (c == '>')
		return (e_state_dgt);
	else if (c == '<')
		return (e_state_dlt);
	return (e_state_delim);
}


int		sc_state_delimiter(char *str)
{
	if (ft_strlen(str) > 1)
	{
		if (ft_strncmp(str, "&&", 2) == 0)
			return (1);
		else if (ft_strncmp(str, "||", 2) == 0)
			return (1);
		else if (ft_strncmp(str, ">>", 2) == 0)
			return (1);
		else if (ft_strncmp(str, "<<", 2) == 0)
			return (1);
	}
	return (0);
}

t_list *ft_tokenizer(char *str)
{
	enum e_state state;
	int i;
	char *start;
	char *end;
	char *t;
	t_list *tokens;

	tokens = NULL;
	start = str;
	state = get_state(str[0]);
	i = 0;
	while (str[i] != '\0')
	{
		//start != ' ' means that previous state wasn't wspace state
		//also means that the the token is not whitespace
		if (state != get_state(str[i]))
		{
			end = str + i;
			if (*start != ' ')
			{
				t = (char*)malloc(sizeof(char) * (end - start + 1));
				ft_strlcpy(t, start, end - start + 1);
				ft_lstadd_back(&tokens, ft_lstnew(t));
			}
			state = get_state(str[i]);
			start = end;
		}
		if (get_state(str[i]) != e_state_nsc && str[i] != '&' && str[i] != '|'
				&& str[i] != '>' && str[i] != '<')
		{
			state = e_state_delim;
		}
		else if (i - 1 >= 0 && get_state(str[i - 1]) == state 
				&& state != e_state_nsc)
		{
			state = get_dstate(str[i]);
		}
		i++;
	}
	return (tokens);
}

//char *get_token(char *sub_str)
//{
//	int i;
//	char *t;
//
//	i = 0;
//	while (sub_str[i] != ' ' && sub_str[i] != '\0')
//		i++;
//	sub_str[i] = '\0';
//	t = ft_strdup(sub_str);
//	free(sub_str);
//	return t;
//}


//t_list *ft_tokenizer(char *str)
//{
//	t_list *tokens;
//	char *t;
//
//	tokens = NULL;
//	while (str != NULL && *str != '\0')
//	{
//		while (*str == ' ')
//			str++;
//		if (*str != '\0')
//		{
//			t = get_token(ft_strdup(str));
//			//printf("%s\n", t);
//			ft_lstadd_back(&tokens, ft_lstnew(t));
//		}
//		while (*str != ' ' && *str != '\0')
//			str++;
//	}
//	return tokens;
//}

//t_cmd *fill_cmd(t_list *tokens)
//{
//	t_cmd *cmd;
//
//	cmd = (t_cmd*)malloc(sizeof(t_cmd));
//	cmd->arg_list = NULL;
//	cmd->redir_list = NULL;
//	while (tokens != NULL)
//	{
//		if ((char*)tokens->content)[0] == '>' || (((char*)tokens-content)[0] == '<')
//		{
//		}
//	}
//	return 
//}

int		main()
{
	char *str;
	t_list *tokens;

	str = "echo hello>>>||((((;;;;))))h world , >";
	tokens = ft_tokenizer(str);
	while (tokens != NULL)
	{
		printf("%s\n", tokens->content);
		tokens = tokens->next;
	}
	return (0);
}
