/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/04 12:21:00 by abel-mak          #+#    #+#             */
/*   Updated: 2021/03/04 12:27:42 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/parser.h"

void print_cmd(t_cmd *cmd)
{
	t_list *redir_tmp;
	t_list *word_tmp;

	redir_tmp = (t_list *)cmd->redir_list;
	word_tmp = (t_list *)cmd->word_list;
	printf("\e[1;35m	words\n\e[0m");
	while (word_tmp != NULL)
	{
		printf("%s\n", (char *)word_tmp->content);
		word_tmp = word_tmp->next;
	}
	printf("\e[1;1m	-----redir\n\e[0m");
	while (redir_tmp != NULL)
	{

		if (((t_redir *)redir_tmp->content)->type == e_state_gt)
			printf("redir type: %s ", "e_state_gt");
		else if (((t_redir *)redir_tmp->content)->type == e_state_lt)
			printf("redir type: %s ", "e_state_lt");
		else if (((t_redir *)redir_tmp->content)->type == e_state_dgt)
			printf("redir type: %s ", "e_state_dgt");
		else //(((t_redir*)redir_tmp->content)->type == e_state_dlt)
			printf("redir type: %s ", "e_state_dlt");
		printf("redir file: %s\n", ((t_redir *)redir_tmp->content)->file);
		redir_tmp = redir_tmp->next;
		printf("+++++++\n");
	}
}

void print_pipe(t_pipe *pipe)
{
	t_list *cmd_list;

	cmd_list = pipe->cmd_list;
	while (cmd_list != NULL)
	{
		print_cmd((t_cmd *)cmd_list->content);
		cmd_list = cmd_list->next;
	}
}

void print_list(t_list *cond_list)
{
	int i;

	i = 0;
	while (cond_list != NULL)
	{
		printf("\e[1;42mlist: %d\e[0m\n", i);
		print_pipe((t_pipe *)cond_list->content);
		cond_list = cond_list->next;
		i++;
	}
}
