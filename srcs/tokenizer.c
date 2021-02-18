/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 10:36:01 by abel-mak          #+#    #+#             */
/*   Updated: 2021/02/18 16:06:32 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"


typedef struct	s_pipe
{
	enum e_state condition;
	t_list *cmd_list;
}				t_pipe;

typedef struct	s_cond
{
	int is_pipe;
	t_list *pipe_list;
}				t_cond;

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
	else if (c == '$')
		return (e_state_dollar);
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
	else if (c == '*')
		return (e_state_wildcard);
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

int is_between_quotes(t_list *tl)
{
	enum e_state quote;
	enum e_state type;

	//because if escape is between quotes it no longer has any effect on tokens
	//example: echo '\''
	quote = 0;
	while (tl != NULL)
	{
		type = ((t_token*)tl->content)->type;
		if (quote == 0 && (type == e_state_squote || type == e_state_dquote))
		{
			quote = type;
		}
		else if (quote != 0 && quote == type)
		{
			quote = 0;
		}
		tl = tl->next;
	}
	//if quotes are single quotes all sc become nsc
	//else if dquote some of them are not switched
	if (quote != 0 && quote != e_state_dquote)
		return (1);
	else
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
				state = get_state(start[0]);
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
				&& check_token_type(ft_lstlast(tokens), e_state_escape) == 1
				&& is_between_quotes(tokens) == 0)
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
			state = get_dstate(start[j]);
		}
		i++;
		j++;
	}
	//if (state != e_state_wspace && *start != '\0')
	if (*start != '\0')
	{
		//printf("%d\n", e_state_nsc);
		//printf("%d\n", state);
		if (state == e_state_delim)
			state = get_state(start[0]);
		ft_lstadd_back(&tokens, ft_lstnew(create_token(ft_strdup(start), state)));
	}
	return (tokens);
}


void	free_token(t_list *elem)
{
	t_token *token;

	token = (t_token*)(elem->content);
	free(token->value);
	free(token);
	free(elem);
}

void	remove_first_token(t_list **tokens_list, enum e_state type)
{
	t_list *tmp;

	tmp = (*tokens_list);
	if (tmp != NULL && ((t_token*)tmp->content)->type == e_state_escape 
			&& tmp->next == NULL)
	{
		//escape error
		printf("\e[0;31mfrom remove_first_token: escape error!!!\n\e[0m");
	}
	else if (tmp != NULL && ((t_token*)tmp->content)->type == type)
	{
		(*tokens_list) = tmp->next;
		free_token(tmp);
		remove_first_token(tokens_list, type);
	}
}

int	remove_token_by_type(t_list **tokens_list, enum e_state type)
{
	t_list *tmp;	
	t_list *escape;

	remove_first_token(tokens_list, type);
	tmp = *tokens_list;
	while (tmp != NULL)
	{
		if (tmp->next != NULL && ((t_token*)tmp->next->content)->type == type)
		{
			if (((t_token*)tmp->next->content)->type == e_state_escape
					&& tmp->next->next == NULL)
			{
				//escape error
				printf("\e[0;31mescape error!!!\n[0m");
			}
			else
			{
				escape = tmp->next;
				tmp->next = tmp->next->next;
				free_token(escape);
				tmp = *tokens_list;
			}
		}
		tmp = tmp->next;
	}
	return (0);
}

//voi$*******8d	remove_token_by_type(t_list *tokens_list, enum e_state type)
//{
//	t_list *tmp;	
//	t_list *escape;
//	
//	tmp = tokens_list;
//	while (tmp != NULL)
//	{
//		if (tmp->next != NULL && ((t_token*)tmp->next->content)->type == type
//				&& tmp->next->next != NULL)
//		{
//			escape = tmp->next;
//			tmp->next = tmp->next->next;
//			free_token(escape);
//			tmp = tokens_list;
//		}
//		else if (tmp->next != NULL && ((t_token*)tmp->next->content)->type == e_state_escape
//			   && tmp->next->next == NULL)
//		{
//			//put error escape token must be followed by non empty token
//		}	
//		tmp = tmp->next;
//	}
//	//return (NULL);
//}

void	join_same_type(t_list *tokens_list, enum e_state type)
{
	t_list *tmp;
	t_list *next_nsc;
	char *tmp_str;

	tmp = tokens_list;
	while (tmp != NULL)
	{
		if (((t_token*)tmp->content)->type == type
				&& tmp->next != NULL 
				&& ((t_token*)tmp->next->content)->type == type)
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

//void	join_nsc(t_list *tokens_list)
//{
//	t_list *tmp;
//	t_list *next_nsc;
//	char *tmp_str;
//
//	tmp = tokens_list;
//	while (tmp != NULL)
//	{
//		if (((t_token*)tmp->content)->type == e_state_nsc
//				&& tmp->next != NULL 
//				&& ((t_token*)tmp->next->content)->type == e_state_nsc)
//		{
//			next_nsc = tmp->next;
//			tmp_str = ((t_token*)tmp->content)->value;
//			((t_token*)tmp->content)->value = ft_strjoin(
//				tmp_str, 
//				((t_token*)next_nsc->content)->value);
//			tmp->next = next_nsc->next;
//			free_token(next_nsc);
//			free(tmp_str);
//			tmp = tokens_list;
//		}
//		else
//			tmp = tmp->next;
//	}
//}

//void	trim_quotes(t_list *tokens_list)
//{
//	t_list			*tmp;
//	enum e_state	type;
//	char *v;
//	char *new_v;
//
//	tmp = tokens_list;
//	while (tmp != NULL)
//	{
//		type = ((t_token*)tmp->content)->type;
//		v = ((t_token*)tmp->content)->value;
//		//check if type is e_state_squote or e_state_dquote
//		//check if len >= 2
//		//check if start and end with same type of quotes
//		if ((type == e_state_squote || type == e_state_dquote)
//				&& ft_strlen(((t_token*)tmp->content)->value) > 1
//				&& get_state(v[0]) == type && get_state(v[ft_strlen(v) - 1]) == type)
//		{
//			new_v = (char*)malloc(sizeof(char) * (ft_strlen(v) - 2) + 1);
//			ft_strlcpy(new_v, v + 1, ft_strlen(v) - 1);
//			free(v);
//			((t_token*)tmp->content)->value = new_v;
//			((t_token*)tmp->content)->type = e_state_nsc;
//		}
//		tmp = tmp->next;
//	}
//}

enum e_state	subs_quotes(t_list *tl, enum e_state quote, enum e_state type)
{	
	t_list *new_token;
	
	if (quote == 0 && (type == e_state_squote || type == e_state_dquote))
	{
		//Mr.Bricolage if quotes are succesive like '' add empty string
		//between them
		if (tl->next != NULL 
				&& ((t_token*)tl->next->content)->type == type)
		{
			new_token = ft_lstnew(create_token(ft_strdup(""), e_state_nsc));
			ft_lstadd_front(&tl->next, new_token);
			tl->next = new_token;
		}
		quote = type;
	}
	else if (quote != 0 && type == quote)
		quote = 0;
	else if (quote != 0 && (quote != e_state_dquote || type != e_state_dollar))
		((t_token*)tl->content)->type = e_state_nsc;
	return (quote);
}


void	quotes(t_list *tokens_list)
{
	enum e_state quote;
	enum e_state type;
//	t_list *new_token;

	quote = 0;
	while (tokens_list != NULL)
	{
		type = ((t_token*)tokens_list->content)->type;
		quote = subs_quotes(tokens_list, quote, type);
		tokens_list = tokens_list->next;
	}
	if(quote != 0)
		printf("\e[0;31mquotes error!!!\n\e[0m");
}

void	subs_dollar(t_list *tl, char **env)
{
	char *env_name;
	enum e_state type;

	if (tl->next != NULL)
	{
		type = ((t_token*)tl->next->content)->type;
		if (type == e_state_nsc || type == e_state_wildcard)
		{
			env_name = ((t_token*)tl->next->content)->value;
			((t_token*)tl->next->content)->value = check_var_env(env, env_name);
			((t_token*)tl->next->content)->type = e_state_nsc;
			free(env_name);
		}
		else if (type == e_state_dollar)
		{
			//replace with process id
		}
	}
	if (tl->next == NULL ||(type != e_state_squote && type != e_state_dquote
			 && type != e_state_nsc))
		((t_token*)tl->content)->type = e_state_nsc;
}

void	dollar(t_list *tl, char **env)
{
	//char *env_name;

	while (tl != NULL)
	{
		if (((t_token*)tl->content)->type == e_state_dollar)
		{
			subs_dollar(tl, env);
		}
		tl = tl->next;
	}
}

t_list *fill_list(t_list *tokens_list, t_list **cond_list);
void    print_list(t_list *cond_list);

void print_cmd(t_cmd *cmd)
{
	t_list *redir_tmp;
	t_list *word_tmp;
	t_list *subshell;
	int random;

	random = rand();
	redir_tmp = (t_list*)cmd->redir_list;
	word_tmp = (t_list*)cmd->word_list;
	subshell = (t_list*)cmd->subshell;
	if (subshell != NULL)
	{
		printf("\e[33;40m	%d: beginSubshell\e[0m\n", random);
		print_list(subshell);
		printf("\e[33;40m	%d: endSubshell\e[0m\n", random);
	}
	else
	{
		printf("\e[1;35m	words\n\e[0m");
		while (word_tmp != NULL)
		{
			printf("%s\n", (char*)word_tmp->content);
			word_tmp = word_tmp->next;
		}
	}
	printf("\e[1;1m	-----redir\n\e[0m");
	while (redir_tmp != NULL)
	{

		if (((t_redir*)redir_tmp->content)->type == e_state_gt)
			printf("redir type: %s ", "e_state_gt");
		else if (((t_redir*)redir_tmp->content)->type == e_state_lt)
			printf("redir type: %s ", "e_state_lt");
		else if (((t_redir*)redir_tmp->content)->type == e_state_dgt)
			printf("redir type: %s ", "e_state_dgt");
		else //(((t_redir*)redir_tmp->content)->type == e_state_dlt)
			printf("redir type: %s ", "e_state_dlt");
		printf("redir file: %s\n", ((t_redir*)redir_tmp->content)->file);
		redir_tmp = redir_tmp->next;
		printf("+++++++\n");
	}
}

t_redir *get_redir(t_list *tl)
{
	t_redir *redir;

	redir = (t_redir*)malloc(sizeof(t_redir));
	redir->type = ((t_token*)tl->content)->type;
	redir->file = ft_strdup(((t_token*)tl->next->content)->value);
	return (redir);
}

int is_redir(enum e_state type)
{
	if (type == e_state_gt || type == e_state_lt || type == e_state_dgt
			|| type == e_state_dlt)
		return (1);
	return (0);
}


t_list	*fill_subshell(t_list *tl, t_cmd **cmd)
{
	t_list *cond_list;

	printf("found m**f** openparent: %s\n", ((t_token*)tl->content)->value);
	tl = fill_list(tl->next, &cond_list);
	(*cmd)->subshell = cond_list;
	//	print_list(cond_list);
	return (tl->next);	
}

t_list	*fill_cmd(t_list *tl, t_cmd **cmd)
{
	enum e_state	type;

	*cmd = (t_cmd*)malloc(sizeof(t_cmd));
	(*cmd)->word_list = NULL;
	(*cmd)->redir_list = NULL;
	(*cmd)->subshell = NULL;
	if (((t_token*)tl->content)->type == e_state_openparen)
		tl = fill_subshell(tl, cmd);//tl = fill_subshell(tl, cmd);
	while (tl != NULL)
	{
		type = ((t_token*)tl->content)->type;
		if (type != e_state_nsc && is_redir(type) == 0)
			break;
		if (type == e_state_nsc)
		{
			ft_lstadd_back(&(*cmd)->word_list, 
					ft_lstnew(ft_strdup(((t_token*)tl->content)->value)));
		}
		if (is_redir(type) == 1)
		{
			printf("redirections found!\n");
			if (tl->next != NULL)
				ft_lstadd_back(&(*cmd)->redir_list, ft_lstnew(get_redir(tl)));
			else
				printf("\e[0;31mfill_cmd: redirection error!!!\n\e[0m");
			tl = (tl->next != NULL) ? tl->next->next : NULL;
		}
		else
			tl = tl->next;
	}
	return (tl);
}

void	print_pipe(t_pipe *pipe)
{
	t_list *cmd_list;

	cmd_list = pipe->cmd_list;
	while (cmd_list != NULL)
	{
		print_cmd((t_cmd*)cmd_list->content);
		cmd_list = cmd_list->next;
	}
}

t_list *fill_pipe(t_list *tokens_list, t_pipe **pipe, enum e_state condition)
{
	t_cmd *cmd;
	t_list *redir_tmp;
	t_list *word_tmp;
	//t_list *pipe;

	//cmd = NULL;
	tokens_list = fill_cmd(tokens_list, &cmd);
	*pipe = (t_pipe*)malloc(sizeof(t_pipe));
	(*pipe)->cmd_list = NULL;
	(*pipe)->cmd_list = ft_lstnew(cmd);
	(*pipe)->condition = condition;
	//print_cmd(cmd);
	while ((tokens_list != NULL) && ((t_token*)tokens_list->content)->type == e_state_pipe)
	{
		tokens_list = fill_cmd(tokens_list->next, &cmd);
		ft_lstadd_back(&(*pipe)->cmd_list, ft_lstnew(cmd));
		//print_cmd(cmd);
	}
	return (tokens_list);
}

t_list	*fill_cond(t_list *tokens_list, t_cond **cond)//, int is_pipe)
{
	t_pipe	*pipe;

	(*cond) = (t_cond*)malloc(sizeof(t_cond));
	tokens_list = fill_pipe(tokens_list, &pipe, 0);
	(*cond)->pipe_list = ft_lstnew(pipe);
	while (tokens_list != NULL 
			&& (((t_token*)tokens_list->content)->type == e_state_dpipe
				|| ((t_token*)tokens_list->content)->type == e_state_dand))
	{
		tokens_list = fill_pipe(tokens_list->next, &pipe, ((t_token*)tokens_list->content)->type);
		//(*cond)->is_pipe = is_pipe;
		ft_lstadd_back(&(*cond)->pipe_list, ft_lstnew(pipe));
	}
	return (tokens_list);
}

void	print_cond(t_cond *cond)
{
	t_list *pipe_list;
	int i;

	pipe_list = cond->pipe_list;
	i = 0;
	while (pipe_list != NULL)
	{
		if (i == 0)
			printf("\e[33;7mcondition: %d\n\e[0m\n", i);
		else if (((t_pipe*)pipe_list->content)->condition == e_state_dpipe)
			printf("\e[33;7mcondition: %d type: e_state_dpipe\n\e[0m\n", i);
		else
			printf("\e[33;7mcondition: %d type: e_state_dand\n\e[0m\n", i);
		print_pipe((t_pipe*)pipe_list->content);
		pipe_list = pipe_list->next;
		i++;
	}
}


t_list *fill_list(t_list *tokens_list, t_list **cond_list)
{
	t_cond *cond;

	tokens_list = fill_cond(tokens_list, &cond);
	*cond_list =ft_lstnew(cond);
	while (tokens_list != NULL
			&& ((t_token*)tokens_list->content)->type == e_state_scolon)
	{
		if (tokens_list->next == NULL)
			tokens_list = NULL;
		else
		{
			tokens_list = fill_cond(tokens_list->next, &cond);
			ft_lstadd_back(cond_list, ft_lstnew(cond));
		}
	}
	return (tokens_list);
}

void	print_list(t_list *cond_list)
{
	int i;

	i = 0;
	while (cond_list != NULL)
	{
		printf("\e[1;42mlist: %d\e[0m\n", i);
		print_cond((t_cond*)cond_list->content);
		cond_list = cond_list->next;
		i++;
	}
}

void 	parse(t_list *tokens_list)
{
	t_pipe *pipe;
	t_list *conditional;
	int i;
	t_cond *cond;
	t_list *pipe_list;
	t_list *cond_list;

	i = 0;
	fill_list(tokens_list, &cond_list);
	i = 0;
	print_list(cond_list);
	//	while (cond_list != NULL)
	//	{
	//		printf("\e[1;42mlist: %d\e[0m\n", i);
	//		print_cond((t_cond*)cond_list->content);
	//		cond_list = cond_list->next;
	//		i++;
	//	}
}

//replace every duplicated 'c' to one e.i ***** -> *

char	*change_to_one(char *str, char c)
{
	char *res;
	int i;
	int j;

	res = (char*)malloc(sizeof(char) * ft_strlen(str) + 1);
	ft_bzero(res, ft_strlen(str) + 1);
	i = 0;
	j = 0;
	while (i < ft_strlen(str))
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

void 	create_pattern(t_list *tl)
{
	enum e_state type;
	enum e_state next_type;

	//change_to_one_wild(tl);
	while (tl != NULL)
	{
		if (tl->next != NULL)
		{
			type = ((t_token*)tl->content)->type;
			next_type = ((t_token*)tl->next->content)->type;
			if ((type == e_state_wildcard && next_type == e_state_nsc)
					|| (type == e_state_nsc && next_type == e_state_wildcard))
			{
				((t_token*)tl->content)->type = e_state_wildcard;
				((t_token*)tl->next->content)->type = e_state_wildcard;
			}
		}
		tl = tl->next;
	}
}

//int match(char *pattern, char *string, int p, int s);
//char **get_dir_arr();
//void free_dir_arr(char **dir_arr);

t_list  *matched_dir_list_test(char *pattern);

void subs_wildcard(t_list *tl)
{
	char **dir_arr;
	t_list *dir_list;
	t_list *wild_tmp;

	dir_arr = get_dir_arr();
//	if ((dir_list = matched_dir_list(dir_arr, 
//					((t_token*)tl->next->content)->value)) != NULL)
	if ((dir_list = matched_dir_list_test
				(((t_token*)tl->next->content)->value)) != NULL)
	{
		wild_tmp = tl->next;
		tl->next = dir_list;
		(ft_lstlast(dir_list))->next = wild_tmp->next;
		free_token(wild_tmp);
	}
	else
		((t_token*)tl->next->content)->type = e_state_nsc;
	if (dir_arr)
		free_dir_arr(dir_arr);
}

void	wildcard(t_list **tl)
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
		if (tmp->next != NULL &&
				((t_token*)tmp->next->content)->type == e_state_wildcard)
			subs_wildcard(tmp);
		tmp = tmp->next;
	}
	*tl = head->next;
	free_token(head);
}

int		main(int argc, char **argv, char **env)
{
	char *str;
	t_list *tokens_list;
	char *line;
	t_cmd *cmd;
	t_list *tmp;
	t_token *token;
	int r;
	t_list *redir_tmp;
	t_list *word_tmp;
	//printf("%p\n", env[31]);
	//printf("%s\n", check_var_env(env, "$"));
	//printf("%d\n", getppid());
	r = 1;
	while (r == 1)
	{
		//str = "echo  '''''\"\"\"\"\"             \"\"\"\"'''''\"\\&\\&\\\\ '' \\&&&&;\" +";
		r =	get_next_line(0, &line);
		str = line;
		tokens_list = ft_tokenizer(str);
		printf("\e[0;35mfirst step: simple cut by state\n\e[0m");
		tmp = tokens_list;
		while (tmp != NULL)
		{
			token = (t_token*)tmp->content;
			printf("|%s|:", token->value);
			if (token->type != e_state_nsc)
				printf("\e[1;32m sc: %d\n\e[0m", token->type);
			else
				printf("\e[1;32m nsc\n\e[0m");
			tmp = tmp->next;
		}
		printf("\n");
		printf("===========================\n");
		printf("\e[0;35msecond step: change everthing between quotes to e_state(squote | dquote(except dollar and escape))\n\e[0m");
		quotes(tokens_list);
		tmp = tokens_list;
		while (tmp != NULL)
		{
			token = (t_token*)tmp->content;
			printf("|%s|:", token->value);
			if (token->type != e_state_nsc)
				printf("\e[1;32m sc: %d\n\e[0m", token->type);
			else
				printf("\e[1;32m nsc\n\e[0m");
			tmp = tmp->next;
		}
		printf("===========================\n");
		printf("\e[0;35mthird step: replace what after dollar\n\e[0m");
		dollar(tokens_list, env);
		tmp = tokens_list;
		while (tmp != NULL)
		{
			token = (t_token*)tmp->content;
			printf("|%s|:", token->value);
			if (token->type != e_state_nsc)
				printf("\e[1;32m sc: %d\n\e[0m", token->type);
			else
				printf("\e[1;32m nsc\n\e[0m");
			tmp = tmp->next;
		}
		printf("===========================\n");
		printf("\e[0;35mforth step: remove token type (squote | dquote)\n\e[0m");
		remove_token_by_type(&tokens_list, e_state_squote);
		remove_token_by_type(&tokens_list, e_state_dquote);
		tmp = tokens_list;
		while (tmp != NULL)
		{
			token = (t_token*)tmp->content;
			printf("|%s|:", token->value);
			if (token->type != e_state_nsc)
				printf("\e[1;32m sc: %d\n\e[0m", token->type);
			else
				printf("\e[1;32m nsc\n\e[0m");
			tmp = tmp->next;
		}
		printf("===========================\n");
		printf("\e[0;35mfifth step: remove token type escape\n\e[0m");
		remove_token_by_type(&tokens_list, e_state_escape);
		tmp = tokens_list;
		while (tmp != NULL)
		{
			token = (t_token*)tmp->content;
			printf("|%s|:", token->value);
			if (token->type != e_state_nsc)
				printf("\e[1;32m sc: %d\n\e[0m", token->type);
			else
				printf("\e[1;32m nsc\n\e[0m");
			tmp = tmp->next;
		}
		printf("===========================\n");
		printf("\e[0;35msixth step: remove token type dollar\n\e[0m");
		remove_token_by_type(&tokens_list, e_state_dollar);
		tmp = tokens_list;
		while (tmp != NULL)
		{
			token = (t_token*)tmp->content;
			printf("|%s|:", token->value);
			if (token->type != e_state_nsc)
				printf("\e[1;32m sc: %d\n\e[0m", token->type);
			else
				printf("\e[1;32m nsc\n\e[0m");
			tmp = tmp->next;
		}
		printf("===========================\n");
		printf("\e[0;35mseven step: join token type wildcard && call make_pattern on tokens && join\n\e[0m");
		//make pattern will change state of nsc tokens if they are neighbors of wildcard
		join_same_type(tokens_list, e_state_wildcard);
		create_pattern(tokens_list);
		join_same_type(tokens_list, e_state_wildcard);
		tmp = tokens_list;
		while (tmp != NULL)
		{
			token = (t_token*)tmp->content;
			printf("|%s|:", token->value);
			if (token->type != e_state_nsc)
				printf("\e[1;32m sc: %d\n\e[0m", token->type);
			else
				printf("\e[1;32m nsc\n\e[0m");
			tmp = tmp->next;
		}
		printf("===========================\n");
		printf("\e[0;35meight step: join tokens type nsc\n\e[0m");
		join_same_type(tokens_list, e_state_nsc);
		tmp = tokens_list;
		while (tmp != NULL)
		{
			token = (t_token*)tmp->content;
			printf("|%s|:", token->value);
			if (token->type != e_state_nsc)
				printf("\e[1;32m sc: %d\n\e[0m", token->type);
			else
				printf("\e[1;32m nsc\n\e[0m");
			tmp = tmp->next;
		}
		printf("===========================\n");
		printf("\e[0;35mchange every pattern(token type wildcard) with correspanding matched list\n\e[0m");
		wildcard(&tokens_list);
		tmp = tokens_list;
		while (tmp != NULL)
		{
			token = (t_token*)tmp->content;
			printf("|%s|:", token->value);
			if (token->type != e_state_nsc)
				printf("\e[1;32m sc: %d\n\e[0m", token->type);
			else
				printf("\e[1;32m nsc\n\e[0m");
			tmp = tmp->next;
		}
		printf("===========================\n");
		printf("\e[0;35mfinal step: remove token type wspace\n\e[0m");
		remove_token_by_type(&tokens_list, e_state_wspace);
		tmp = tokens_list;
		while (tmp != NULL)
		{
			token = (t_token*)tmp->content;
			printf("|%s|:", token->value);
			if (token->type != e_state_nsc)
				printf("\e[1;32m sc: %d\n\e[0m", token->type);
			else
				printf("\e[1;32m nsc\n\e[0m");
			tmp = tmp->next;
		}
		//echo '"''"'""''"'"
		//echo $'"""''""'""''''""'"'
		parse(tokens_list);
		printf("\e[0;33m%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\e[0m\n");

		free(line);
	}
	return (0);
}
