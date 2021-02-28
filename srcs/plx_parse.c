/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plx_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 16:36:14 by abel-mak          #+#    #+#             */
/*   Updated: 2021/02/28 17:50:08 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

t_redir *get_redir(t_list *tl)
{
	t_redir *redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	redir->type = ((t_token *)tl->content)->type;
	redir->file = ft_strdup(((t_token *)tl->next->content)->value);
	return (redir);
}

t_list *fill_cmd(t_list *tl, t_cmd **cmd)
{
	enum e_state type;

	*cmd = (t_cmd *)malloc(sizeof(t_cmd));
	(*cmd)->word_list = NULL;
	(*cmd)->redir_list = NULL;
	while (tl != NULL)
	{
		type = ((t_token *)tl->content)->type;
		if (type != e_state_nsc && is_redir(type) == 0)
			break;
		if (type == e_state_nsc)
		{
			ft_lstadd_back(&(*cmd)->word_list,
						   ft_lstnew(ft_strdup(((t_token *)tl->content)->value)));
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

t_list *fill_pipe(t_list *tokens_list, t_pipe **pipe)
{
	t_cmd *cmd;

	*pipe = (t_pipe *)malloc(sizeof(t_pipe));
	tokens_list = fill_cmd(tokens_list, &cmd);
	(*pipe)->cmd_list = ft_lstnew(cmd);

	while ((tokens_list != NULL) 
			&& ((t_token *)tokens_list->content)->type == e_state_pipe)
	{
		tokens_list = fill_cmd(tokens_list->next, &cmd);
		ft_lstadd_back(&(*pipe)->cmd_list, ft_lstnew(cmd));
	}
	return (tokens_list);
}

t_list *fill_list(t_list **tl, t_list **cond_list, t_exec *exec)
{
	//	t_cond *cond;
	t_pipe *pipe;
	t_list *tmp;
	// t_token *token;
	// t_cmd *tmp__cmd;

	tmp = replace_afterdollar(tl, exec);
	tmp = fill_pipe(tmp, &pipe);
	*cond_list = ft_lstnew(pipe);
	printf("\e[0;33m%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\e[0m\n");
	execution_part(pipe, exec, *tl, *cond_list);
	while (tmp != NULL && ((t_token *)tmp->content)->type == e_state_scolon)
	{
		if (tmp->next == NULL)
			tmp = NULL;
		else
		{
			tmp->next = replace_afterdollar(&(tmp->next), exec);
			tmp = fill_pipe(tmp->next, &pipe);
			execution_part(pipe, exec, *tl, *cond_list);
			ft_lstadd_back(cond_list, ft_lstnew(pipe));
		}
	}
	return (tmp);
}

void parse(t_list **tokens_list, t_exec *exec, int *error)
{
	// t_pipe *pipe;
	// t_list *conditional;
	// t_cond *cond;
	// t_list *pipe_list;
	t_list *cond_list;
	// t_list *tmp;
	//(void)tokens_list;
	//(void)exec;
	//(void)error;
	//	i = 0;
	//	fill_list(tokens_list, &cond_list);
	//	i = 0;
	//	print_list(cond_list);
	//--------------------------------
	//printf("%p", tokens_list);
	if (syntax_list(*tokens_list, error) == 0 && *tokens_list != NULL)
	{
		fill_list(tokens_list, &cond_list, exec);
		free_tokens_list(*tokens_list);
		free_list(cond_list);
		//print_list(cond_list);
	}
	else if (*error != 0)
		exec->code_ret = 258;
	//i = 0;
	//--------------------------------
	//	tmp = tokens_list;
	//	tmp = fill_list_test(tmp, &pipe);
	//	print_pipe(pipe);
	//-------------------------------
	//	while (cond_list != NULL)
	//	{
	//		printf("\e[1;42mlist: %d\e[0m\n", i);
	//		print_cond((t_cond*)cond_list->content);
	//		cond_list = cond_list->next;
	//		i++;
	//	}
}
