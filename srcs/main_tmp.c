/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 08:54:21 by asaadi            #+#    #+#             */
/*   Updated: 2021/03/03 17:22:42 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
int var_glob;
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

int check_line(char *line)
{
	int check_;

	check_ = 0;
	while (*line)
	{
		if (*line != ' ')
			check_ = 1;
		line++;
	}
	return (check_);
}

void signals_check(int sig)
{
	if (sig == SIGQUIT)
	{
		var_glob = 1;
		puts("Quit 3:");
	}
}

/*
 * errors:
 * 2 = quotes error,
 */

int main(int ac, char **av, char **env)
{
	t_list *tokens_list;
	t_exec exec;
	char *line;
	t_list *tmp;
	t_token *token;
	int r;
	char *str;
	int error;

	exec.envp = envp_cpy(env);
	exec.code_ret = 0;
	(void)ac;
	(void)av;
	r = 1;
	var_glob = 0;
	signal(SIGQUIT, signals_check); // check if
	while (r == 1)
	{
		write(2, "\033[0;33mminishell-4.2$ \033[0m", ft_strlen("\033[1;32mminishell-4.2 $>\033[0m"));
		r = get_next_line(0, &line);
		error = 0;
		//if (check_line(line))
		//{
			// printf("r====%d {%s}\n", r, line);
			str = line;
			tokens_list = ft_tokenizer(str);
			printf("\e[0;35mfirst step: simple cut by state\n\e[0m");
			tmp = tokens_list;
			while (tmp != NULL)
			{
				token = (t_token *)tmp->content;
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
			quotes(tokens_list, &error);
			tmp = tokens_list;
			while (tmp != NULL)
			{
				token = (t_token *)tmp->content;
				printf("|%s|:", token->value);
				if (token->type != e_state_nsc)
					printf("\e[1;32m sc: %d\n\e[0m", token->type);
				else
					printf("\e[1;32m nsc\n\e[0m");
				tmp = tmp->next;
			}
			printf("===========================\n");
			printf("\e[0;35mforth step: remove token type (squote | dquote)\n\e[0m");
			remove_token_by_type(&tokens_list, e_state_squote, 0);
			remove_token_by_type(&tokens_list, e_state_dquote, 0);
			tmp = tokens_list;
			while (tmp != NULL)
			{
				token = (t_token *)tmp->content;
				printf("|%s|:", token->value);
				if (token->type != e_state_nsc)
					printf("\e[1;32m sc: %d\n\e[0m", token->type);
				else
					printf("\e[1;32m nsc\n\e[0m");
				tmp = tmp->next;
			}
			printf("===========================\n");
			printf("\e[0;35mthird step: replace what after dollar\n\e[0m");
			dollar(tokens_list);
			tmp = tokens_list;
			while (tmp != NULL)
			{
				token = (t_token *)tmp->content;
				printf("|%s|:", token->value);
				if (token->type != e_state_nsc)
					printf("\e[1;32m sc: %d\n\e[0m", token->type);
				else
					printf("\e[1;32m nsc\n\e[0m");
				tmp = tmp->next;
			}
			printf("===========================\n");
			printf("\e[0;35mfifth step: remove token type escape\n\e[0m");
			remove_token_by_type(&tokens_list, e_state_escape, 0);
			tmp = tokens_list;
			while (tmp != NULL)
			{
				token = (t_token *)tmp->content;
				printf("|%s|:", token->value);
				if (token->type != e_state_nsc)
					printf("\e[1;32m sc: %d\n\e[0m", token->type);
				else
					printf("\e[1;32m nsc\n\e[0m");
				tmp = tmp->next;
			}
			printf("===========================\n");
			printf("\e[0;35msixth step: remove token type dollar\n\e[0m");
			remove_token_by_type(&tokens_list, e_state_dollar, 0);
			tmp = tokens_list;
			while (tmp != NULL)
			{
				token = (t_token *)tmp->content;
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
			join_same_type(tokens_list, e_state_wildcard, 0);
			create_pattern(tokens_list);
			join_same_type(tokens_list, e_state_wildcard, 0);
			tmp = tokens_list;
			while (tmp != NULL)
			{
				token = (t_token *)tmp->content;
				printf("|%s|:", token->value);
				if (token->type != e_state_nsc)
					printf("\e[1;32m sc: %d\n\e[0m", token->type);
				else
					printf("\e[1;32m nsc\n\e[0m");
				tmp = tmp->next;
			}
			printf("===========================\n");
			printf("\e[0;35meight step: switch qsm to nsc, join tokens type nsc\n\e[0m");
			switch_state(tokens_list, e_state_qsm, e_state_nsc);
			join_same_type(tokens_list, e_state_nsc, 0);
			tmp = tokens_list;
			while (tmp != NULL)
			{
				token = (t_token *)tmp->content;
				printf("|%s|:", token->value);
				if (token->type != e_state_nsc)
					printf("\e[1;32m sc: %d\n\e[0m", token->type);
				else
					printf("\e[1;32m nsc\n\e[0m");
				tmp = tmp->next;
			}
			parse(&tokens_list, &exec, &error);
			printf("\e[0;33m%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\e[0m\n");
			printf("\e[0;32m____return___ %d\e[0m\n", exec.code_ret);
			ft_free_arr((void **)&line);
			//while(1);
		//}
	}
	return (0);
}
