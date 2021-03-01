/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 08:54:21 by asaadi            #+#    #+#             */
/*   Updated: 2021/03/01 19:15:22 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*
** CTRL + D = EOF
** Special character on input, which is recognized if the ICANON flag is set.
** When received, all the bytes waiting to be read are immediately\
** passed to the process without waiting for a <newline>, and the EOF is discarded.
** Thus, if there are no bytes waiting (that is, the EOF occurred at the beginning of a line),\
** a byte count of zero shall be returned from the read(), representing an end-of-file indication.
** If ICANON is set, the EOF character shall be discarded when processed. 
*/

void	lexer(char *line, t_exec *exec)
{
	t_list	*tokens_list;
	int		error;

	if (exec->r == 0)
		return ;
	error = 0;
	tokens_list = ft_tokenizer(line);
	quotes(tokens_list, &error);
	remove_token_by_type(&tokens_list, e_state_squote, 0);
	remove_token_by_type(&tokens_list, e_state_dquote, 0);
	dollar(tokens_list);
	remove_token_by_type(&tokens_list, e_state_escape, 0);
	remove_token_by_type(&tokens_list, e_state_dollar, 0);
	join_same_type(tokens_list, e_state_wildcard, 0);
	create_pattern(tokens_list);
	join_same_type(tokens_list, e_state_wildcard, 0);
	switch_state(tokens_list, e_state_qsm, e_state_nsc);
	join_same_type(tokens_list, e_state_nsc, 0);
	parse(&tokens_list, exec, &error);
}

int main(int ac, char **av, char **env)
{
	t_exec exec;
	char *str;
	char *line;
	// int i;


	(void)av;
	(void)ac;
	exec.envp = envp_cpy(env);
	exec.code_ret = 0;
	exec.r = 1;
	g_var = 0;
	g_sig = 0;
	signal(SIGQUIT, sig_handler);
	signal(SIGINT, sig_handler);
	while (exec.r == 1/*|| exec.r == 2*/)
	{
		ft_putstr_fd("\033[0;33mminishell-42$ \033[0m", 1);
		exec.r = get_next_line(0, &line);
		if (g_sig == CC && !g_var)
    		exec.code_ret = 1;
		str = line;
		lexer(line, &exec);
		get_return_signals(&exec);
		// if (exec.r == 0)
			// handling_ctrl_d(&exec, line);
		ft_free_arr((void **)&line);
		// if (exec.r != 2)
		// {
		// 	ft_putstr_fd("\033[0;33mminishell-42$ \033[0m", 1);
		// 	exec.r = get_next_line(0, &line);
		// 	if (exec.r == 0)
		// 	{
		// 		i = 0;
		// 		while ((size_t)i < ft_strlen(line))
		// 		{
		// 			ft_putstr_fd("\b\b  \b\b", 1);
		// 			// ft_putstr_fd(" \b", 1);
		// 			i++;
		// 		}
		// 		ft_putstr_fd(line, 1);
		// 	}
		// 	//printf("%s\n", line);
	    // 	lexer(line, &exec);
		// 	//if (exec.r == 0)
		// 	//ft_putstr_fd("  \b\b  \b\b", 1);
		// }
		// if (g_sig == CC && !g_var)
    	// 	exec.code_ret = 1;
		// str = line;
		// get_return_signals(&exec);
		// if (exec.r == 0 || exec.r == 2)
		// 	handling_ctrl_d(&exec, &line);
		// if (exec.r != 2)
		// 	ft_free_arr((void **)&line);
		// //printf("r --- %d\n", exec.r);
	}
	return (0);
}
