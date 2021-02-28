/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 08:54:21 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/28 18:22:57 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

/*
** char **envp_cpy(char **env).
** function create a copy of envp.
** end use that copy in all the program.
*/

char **envp_cpy(char **env)
{
	char **envp;
	int i;

	if (!(envp = (char **)malloc(sizeof(char *) * count_vars_env(env) + 1)))
		return (NULL);
	i = 0;
	while (env[i])
	{
		envp[i] = ft_strdup(env[i]);
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

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
	exec.envp = envp_cpy(env);
	exec.code_ret = 0;
	(void)ac;
	(void)av;
	char *str;
	char *line;
	// t_list *tmp;
	// t_token *token;
	exec.r = 1;
	g_var = 0;
	g_sig = 0;
	if (signal(SIGQUIT, sig_handler) == SIG_ERR)
	{
		//errno.
	}
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
		//errno. Ctrl + \\;
	}
	// signal(SIGINT,SIG_DFL);
	while (exec.r == 1)
	{
		ft_putstr_fd("\033[0;33mminishell-42$ \033[0m", 1);
		exec.r = get_next_line(0, &line);
		if (g_sig == CC && !g_var)
    		exec.code_ret = 1;
		str = line;
		lexer(line, &exec);
		get_return_signals(&exec);
		ft_free_arr((void **)&line);
		if (exec.r == 0)
			handling_ctrl_d(&exec);
	}
	return (0);
}
