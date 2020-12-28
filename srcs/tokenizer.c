/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 09:09:54 by abel-mak          #+#    #+#             */
/*   Updated: 2020/12/28 18:17:16 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "../libft/libft.h"

typedef struct	s_cmd
{
	t_list			*word_list;
	t_list			*redir_list;
	struct s_cmd	*next;
}				t_cmd;

typedef struct	s_redir
{
	int	type;
	char *file;
}				t_redir;


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

typedef struct	s_token
{
	enum e_state type;
	char *value;
}				t_token;

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

t_token *create_token(char *value, enum e_state type)
{
	t_token *token;

	token = (t_token*)malloc(sizeof(t_token));
	token->value = value;
	token->type = type;
	return (token);
}

int		check_token_type(t_list *elem, enum e_state type)
{
	if (elem != NULL && ((t_token*)elem->content)->type == type)
		return (1);
//	printf("%s\n", ((t_token*)elem->content)->value);
//	printf("%d\n", ((t_token*)elem->content)->type);
	return (0);
}

t_list *ft_tokenizer(char *str)
{
	enum e_state state;
	int i;
	int j;
	char *start;
	char *end;
	char *t;
	t_list *tokens;

	tokens = NULL;
	start = str;
	state = get_state(str[0]);
	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		//change of state means we must cut
		//start != ' ' means that previous state wasn't wspace state
		//also means that the the token is not whitespace
		if (state != get_state(str[i]))
		{
			//this condition is to get back the real state because e_state_delim
			//just to force change of state and cut
			if (state == e_state_delim)
				state = get_state(str[i - 1]);
			end = start + j;
			//if (state != e_state_wspace)
			//{
				t = (char*)malloc(sizeof(char) * (end - start + 1));
				ft_strlcpy(t, start, end - start + 1);
				ft_lstadd_back(&tokens, ft_lstnew(create_token(t, state)));
			//}
			state = get_state(str[i]);
			start = end;
			//str = start;
			j = 0;
			//i = 0;
		}
		// this if else condition will force change of state
		// second if: if current character is special character and the character before
		// 		is escape character then this current character is no longer special character
		// 		and state must be change to non speciat character
		// third if: because & is && is but not &&& so if we get found two & we must change state
		//if (get_state(str[i]) != e_state_nsc && i - 1 >= 0 && str[i - 1] == '\\')
		if (get_state(str[i]) != e_state_nsc 
				&& check_token_type(ft_lstlast(tokens), e_state_escape) == 1)
		{
			state = e_state_nsc;
		}
		else if (get_state(str[i]) != e_state_nsc 
				&& get_state(str[i]) != e_state_wspace && str[i] != '&' 
				&& str[i] != '|'&& str[i] != '>' && str[i] != '<')
		{
			state = e_state_delim;
		}
		else if (j - 1 >= 0 && get_state(start[j - 1]) == state 
				&& state != e_state_nsc && state != e_state_wspace)
		{
			state = get_dstate(start[i]);
		}
		i++;
		j++;
	}
	//if (state != e_state_wspace && *start != '\0')
	if (*start != '\0')
	{
		//printf("%d\n", e_state_nsc);
		//printf("%d\n", state);
		ft_lstadd_back(&tokens, ft_lstnew(create_token(ft_strdup(start), state)));
	}
	return (tokens);
}

t_redir *get_redir(t_list *token, t_list *next_token)
{
	int type;
	t_redir *redir;

	redir = (t_redir*)malloc(sizeof(t_redir));
	type = 0;
	if (ft_strlen(token->content) == 2 && ft_strncmp(token->content, "<<", 2))
		type = '<' + '<';
	else if (ft_strlen(token->content) == 2 && ft_strncmp(token->content, ">>", 2))
		type = '>' + '>';
	else if (ft_strncmp(token->content, "<", 1))
		type = '<';
	else if  (ft_strncmp(token->content, ">", 1))
		type = '>';
	redir->type = type;
	if (next_token == NULL)
		redir->file = ft_strdup("");
	else
		redir->file = ft_strdup(next_token->content);
	return (redir);
}

//t_cmd *fill_cmd(t_list *tokens)
//{
//	t_cmd *cmd;
//
//	cmd = (t_cmd*)malloc(sizeof(t_cmd));
//	cmd->word_list = NULL;
//	cmd->redir_list = NULL;
//	while (tokens != NULL)
//	{
//		if (ft_strncmp(">", tokens->content, 1) == 0 
//				|| ft_strncmp("<", tokens->content, 1) == 0)
//		{
//			printf("redirections found!\n");
//			ft_lstadd_back(&cmd->redir_list, ft_lstnew(
//						get_redir(tokens, tokens->next)));
//			tokens = (tokens->next != NULL) ? tokens->next->next : NULL;
//		}
//		else
//			tokens = tokens->next;
//	}
//	return (cmd);
//}

void	free_token(t_list *elem)
{
	t_token *token;

	token = (t_token*)elem->content;
	free(token->value);
	free(token);
	free(elem);
}

void	remove_escape(t_list *tokens_list)
{
	t_list *tmp;	
	t_list *escape;
	
	tmp = tokens_list;
	while (tmp != NULL)
	{
		if (tmp->next != NULL && ((t_token*)tmp->next->content)->type == e_state_escape 
				&& tmp->next->next != NULL)
		{
			escape = tmp->next;
			tmp->next = tmp->next->next;
			free_token(escape);
			tmp = tokens_list;
		}
		else if (tmp->next != NULL && ((t_token*)tmp->next->content)->type == e_state_escape
			   && tmp->next->next == NULL)
		{
			//put error escape token must be followed by non empty token
		}	
		tmp = tmp->next;
	}
	//return (NULL);
}

void	join_nsc(t_list *tokens_list)
{
	t_list *tmp;
	t_list *next_nsc;
	char *tmp_str;

	tmp = tokens_list;
	while (tmp != NULL)
	{
		if (((t_token*)tmp->content)->type == e_state_nsc
				&& tmp->next != NULL 
				&& ((t_token*)tmp->next->content)->type == e_state_nsc)
		{
			next_nsc = tmp->next;
			tmp_str = ((t_token*)tmp->content)->value;
			((t_token*)tmp->content)->value = ft_strjoin(
				tmp_str, 
				((t_token*)next_nsc->content)->value);
			tmp->next = next_nsc->next;
			free_token(next_nsc);
			free(tmp_str);
			tmp = tokens_list;
		}
		else
			tmp = tmp->next;
	}
}

int		main()
{
	char *str;
	t_list *tokens_list;
//	t_cmd *cmd;
	t_list *tmp;
	t_token *token;
	//t_list *redir_tmp;

	str = "echo  \\&\\&\\\\ \\&&&&; +";
	tokens_list = ft_tokenizer(str);
//	cmd = fill_cmd(tokens_list);
//	redir_tmp = (t_list*)cmd->redir_list;
//	while (redir_tmp != NULL)
//	{
//		printf("redir type: %d\n", ((t_redir*)redir_tmp->content)->type);
//		printf("redir file: %s\n", ((t_redir*)redir_tmp->content)->file);
//		redir_tmp = redir_tmp->next;
//	}
//	printf("+++++++\n");
	printf("\e[0;35mfirst step:\n\e[0m");
	tmp = tokens_list;
	while (tmp != NULL)
	{
		token = (t_token*)tmp->content;
		//if (token->type != e_state_escape)
		//{
			printf("|%s|:", token->value);
			if (token->type != e_state_nsc)
				printf("\e[1;32m sc\n\e[0m");
			else
				printf("\e[1;32m nsc\n\e[0m");
		//}
		tmp = tmp->next;
	}
	printf("===========================\n");
	printf("\e[0;35msecond step:\n\e[0m");
	tmp = tokens_list;
	remove_escape(tokens_list);
	while (tmp != NULL)
	{
		token = (t_token*)tmp->content;
		if (token->type != e_state_escape)
		{
			printf("|%s|:", token->value);
			if (token->type != e_state_nsc)
				printf("\e[1;32m sc\n\e[0m");
			else
				printf("\e[1;32m nsc\n\e[0m");
		}
		tmp = tmp->next;
	}
	printf("===========================\n");
	printf("\e[0;35mthird step:\n\e[0m");
	tmp = tokens_list;
	join_nsc(tokens_list);
	while (tmp != NULL)
	{
		token = (t_token*)tmp->content;
		if (token->type != e_state_escape)
		{
			printf("|%s|:", token->value);
			if (token->type != e_state_nsc)
				printf("\e[1;32m sc\n\e[0m");
			else
				printf("\e[1;32m nsc\n\e[0m");
		}
		tmp = tmp->next;
	}
	return (0);
}
