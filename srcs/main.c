/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 08:54:21 by asaadi            #+#    #+#             */
/*   Updated: 2021/03/06 12:33:06 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	lexer(char *line, t_exec *exec)
{
	t_list	*tokens_list;
	int		error;

	if (g_sig == CC && !g_var)
		exec->code_ret = 1;
	if (exec->r == 0)
		return ;
	error = 0;
	tokens_list = ft_tokenizer(line);
	quotes(tokens_list, &error);
	dollar(tokens_list);
	remove_token_by_type(&tokens_list, e_state_squote, 0);
	remove_token_by_type(&tokens_list, e_state_dquote, 0);
	remove_token_by_type(&tokens_list, e_state_escape, 0);
	remove_token_by_type(&tokens_list, e_state_dollar, 0);
	join_same_type(tokens_list, e_state_wildcard, 0);
	create_pattern(tokens_list);
	join_same_type(tokens_list, e_state_wildcard, 0);
	switch_state(tokens_list, e_state_qsm, e_state_nsc);
	join_same_type(tokens_list, e_state_nsc, 0);
	parse(&tokens_list, exec, &error);
}

void	prompt(t_exec *exec, char **env)
{
	char *line;
	char *str;

	exec->code_ret = 0;
	exec->r = 1;
	exec->pipe = 0;
	exec->envp = envp_cpy(env);
	while (exec->r == 1 || exec->r == 2)
	{
		if (exec->r != 2)
		{
			if (exec->code_ret != 130)
				ft_putstr_fd("\033[0;33mminishell-42$ \033[0m", 2);
			exec->r = get_next_line(0, &line);
			if (exec->r != 0)
				lexer(line, exec);
		}
		str = line;
		get_return_signals(exec);
		if (exec->r == 0 || exec->r == 2)
			handling_ctrl_d(exec, &line);
		if (exec->r != 2)
			ft_free_arr((void **)&line);
		exec->pipe = 0;
	}
}

int		main(int ac, char **av, char **env)
{
	t_exec exec;

	(void)av;
	(void)ac;
	g_var = 0;
	g_sig = 0;
	signal(SIGQUIT, sig_handler);
	signal(SIGINT, sig_handler);
	prompt(&exec, env);
	return (0);
}
