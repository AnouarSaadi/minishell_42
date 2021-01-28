/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 08:54:21 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/28 16:41:19 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_cmd(char **args, char **envp)
{
	pid_t _pid;
	int status;

	_pid = 0;
	status = 0;
	_pid = fork();
	if (_pid == -1)
	{
		ft_putendl_fd(strerror(errno), 2);
		exit_function(1);
	}
	else if (_pid > 0)
	{
		waitpid(_pid, &status, 0);
		kill(_pid, SIGPIPE);
	}
	else
	{
        if (execve(args[0], args, envp) == -1)
		{
            ft_putendl_fd(strerror(errno), 2);
			exit_function(1);
		}
	}
}


void do_if_is_not_built_in(char **args, char **envp)
{
	if (get_cmd_path(args, envp))
		exec_cmd(args, envp);
	ft_free_2dem_arr(args);
}

void		check_if_built_in(char **args, char ***envp, int *i)
{
	*i = 0;
	if (!ft_strcmp(args[0], "cd"))
		change_directory(args[1], *envp, i);
	if (!ft_strcmp(args[0], "pwd") ||
		!ft_strcmp(args[0], "PWD"))
	{
		pwd_function();
		*i = 1;
	}
	if (!ft_strcmp(args[0], "echo") ||
		!ft_strcmp(args[0], "ECHO"))
	{
		if (args[1] && !ft_strcmp(args[1], "-n"))
			echo_function(args, 1, i);
		else
			echo_function(args, 0, i);
	}
	if (!ft_strcmp(args[0], "export"))
	{
		if (args[1])
			export_function(envp, args, i);
		else
			sort_print_envp_alpha(*envp, i);
	}
	if (!ft_strcmp(args[0], "unset"))
		unset_function(envp, args, i);
	if (!ft_strcmp(args[0], "env") ||
		!ft_strcmp(args[0], "ENV"))
		env_function(*envp, i);
	if (!ft_strcmp(args[0], "exit"))
	{
		if (!args[1])
			exit_function(0);
		else
			exit_function(ft_atoi(args[1]));
		*i = 1;
	}
}

char **envp_cpy(char **env)
{
	char **envp;
	int i;

	if (!(envp = (char **)malloc(sizeof(char*) * count_vars_env(env) + 1)))
		return (NULL);
	i = 0;
	while (env[i])
	{
		envp[i] = ft_strdup(env[i]);
		i++;
	}
	envp[i] = NULL;
	return(envp);
}

int main(int ac, char **av, char **env)
{
	int i;
	char **args;
	char **envp;

	envp = envp_cpy(env);
	i = 0;
	(void)ac;
	(void)av;
	char *str;
	t_list *tokens_list;
	char *line;
	t_cmd *cmd;
	// t_list *tmp;
	// t_token *token;
	int r;
	t_list *redir_tmp;
	t_list *word_tmp;
	//printf("%p\n", env[31]);
	//printf("%s\n", check_var_env(env, "$"));
	//printf("%d\n", getppid());
	r = 1;
	while (r == 1)
	{
		write(1, "\e[1;32mMINISHELL_42$> \e[0m", ft_strlen("\e[1;32mMINISHELL_42$> \e[0m"));
		//str = "echo  '''''\"\"\"\"\"             \"\"\"\"'''''\"\\&\\&\\\\ '' \\&&&&;\" +";
		r =	get_next_line(0, &line);
		str = line;
		tokens_list = ft_tokenizer(str);
		// printf("\e[0;35mfirst step: simple cut by state\n\e[0m");
		// tmp = tokens_list;
		// while (tmp != NULL)
		// {
			// token = (t_token*)tmp->content;
			// printf("|%s|:", token->value);
			// if (token->type != e_state_nsc)
				// printf("\e[1;32m sc: %d\n\e[0m", token->type);
			// else
				// printf("\e[1;32m nsc\n\e[0m");
			// tmp = tmp->next;
		// }
		// printf("\n");
		// printf("===========================\n");
		// printf("\e[0;35msecond step: change everthing between quotes to e_state(squote | dquote(except dollar and escape))\n\e[0m");
		quotes(tokens_list);
		// tmp = tokens_list;
		// while (tmp != NULL)
		// {
			// token = (t_token*)tmp->content;
			// printf("|%s|:", token->value);
			// if (token->type != e_state_nsc)
				// printf("\e[1;32m sc: %d\n\e[0m", token->type);
			// else
				// printf("\e[1;32m nsc\n\e[0m");
			// tmp = tmp->next;
		// }
		// printf("===========================\n");
		// printf("\e[0;35mthird step: replace what after dollar\n\e[0m");
		subs_dollar(tokens_list, envp);
		// tmp = tokens_list;
		// while (tmp != NULL)
		// {
			// token = (t_token*)tmp->content;
			// printf("|%s|:", token->value);
			// if (token->type != e_state_nsc)
				// printf("\e[1;32m sc: %d\n\e[0m", token->type);
			// else
				// printf("\e[1;32m nsc\n\e[0m");
			// tmp = tmp->next;
		// }
		// printf("===========================\n");
		// printf("\e[0;35mforth step: remove token type (squote | dquote)\n\e[0m");
		remove_token_by_type(&tokens_list, e_state_squote);
		remove_token_by_type(&tokens_list, e_state_dquote);
		// tmp = tokens_list;
		// while (tmp != NULL)
		// {
			// token = (t_token*)tmp->content;
			// printf("|%s|:", token->value);
			// if (token->type != e_state_nsc)
				// printf("\e[1;32m sc: %d\n\e[0m", token->type);
			// else
				// printf("\e[1;32m nsc\n\e[0m");
			// tmp = tmp->next;
		// }
		// printf("===========================\n");
		// printf("\e[0;35mfifth step: remove token type escape\n\e[0m");
		remove_token_by_type(&tokens_list, e_state_escape);
		// tmp = tokens_list;
		// while (tmp != NULL)
		// {
			// token = (t_token*)tmp->content;
			// printf("|%s|:", token->value);
			// if (token->type != e_state_nsc)
				// printf("\e[1;32m sc: %d\n\e[0m", token->type);
			// else
				// printf("\e[1;32m nsc\n\e[0m");
			// tmp = tmp->next;
		// }
		// printf("===========================\n");
		// printf("\e[0;35msixth step: remove token type dollar\n\e[0m");
		remove_token_by_type(&tokens_list, e_state_dollar);
		// tmp = tokens_list;
		// while (tmp != NULL)
		// {
			// token = (t_token*)tmp->content;
			// printf("|%s|:", token->value);
			// if (token->type != e_state_nsc)
				// printf("\e[1;32m sc: %d\n\e[0m", token->type);
			// else
				// printf("\e[1;32m nsc\n\e[0m");
			// tmp = tmp->next;
		// }
		// printf("===========================\n");
		// printf("\e[0;35mseven step: join tokens type nsc\n\e[0m");
		join_same_type(tokens_list, e_state_nsc);
		// tmp = tokens_list;
		// while (tmp != NULL)
		// {
			// token = (t_token*)tmp->content;
			// printf("|%s|:", token->value);
			// if (token->type != e_state_nsc)
				// printf("\e[1;32m sc: %d\n\e[0m", token->type);
			// else
				// printf("\e[1;32m nsc\n\e[0m");
			// tmp = tmp->next;
		// }
		// printf("===========================\n");
		// printf("\e[0;35mfinal step: remove token type wspace\n\e[0m");
		remove_token_by_type(&tokens_list, e_state_wspace);
		// tmp = tokens_list;
		// while (tmp != NULL)
		// {
		// 	token = (t_token*)tmp->content;
		// 	printf("|%s|:", token->value);
		// 	if (token->type != e_state_nsc)
		// 		printf("\e[1;32m sc: %d\n\e[0m", token->type);
		// 	else
		// 		printf("\e[1;32m nsc\n\e[0m");
		// 	tmp = tmp->next;
		// }
		cmd = fill_cmd(tokens_list);
		redir_tmp = (t_list*)cmd->redir_list;
		word_tmp = (t_list*)cmd->word_list;
		// while (redir_tmp != NULL)
		// {

		// 	printf("redir type: %d ", ((t_redir*)redir_tmp->content)->type);
		// 	if (((t_redir*)redir_tmp->content)->type == e_state_gt)
		// 		printf("redir type: %s ", "e_state_gt");
		// 	else if (((t_redir*)redir_tmp->content)->type == e_state_lt)
		// 		printf("redir type: %s ", "e_state_lt");
		// 	else if (((t_redir*)redir_tmp->content)->type == e_state_dgt)
		// 		printf("redir type: %s ", "e_state_dgt");
		// 	else //(((t_redir*)redir_tmp->content)->type == e_state_dlt)
		// 		printf("redir type: %s ", "e_state_dlt");
		// 	printf("redir file: %s\n", ((t_redir*)redir_tmp->content)->file);
		// 	redir_tmp = redir_tmp->next;
		// 	printf("+++++++\n");
		// }
		// printf("\e[1;35m	words\n\e[0m");
		// while (word_tmp != NULL)
		// {
		// 	printf("%s\n", (char*)word_tmp->content);
		// 	word_tmp = word_tmp->next;
		// }
		// // echo '"''"'""''"'"
		// // echo $'"""''""'""''''""'"'
		// printf("\e[0;33m%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\e[0m\n");
		free(line);
		t_cmd *_tmp;
		_tmp = cmd;
		i = ft_lstsize(_tmp->word_list);
		if (!(args = (char**)malloc(sizeof(char*) * (i + 1))))
			return(0);
		i = 0;
		while (_tmp->word_list)
		{
			args[i] = ft_strdup(_tmp->word_list->content);
			i++;
			_tmp->word_list = _tmp->word_list->next;
		}
		args[i] = NULL;
		if (_tmp->redir_list)
		{
			i = ft_lstsize(_tmp->redir_list);
			while(_tmp->redir_list)
			{
				printf("%s\n", (char *)_tmp->redir_list->content);
				_tmp->redir_list = _tmp->redir_list->next;
			}
		}
		check_if_built_in(args, &envp, &i);
		if (i == 0)
			do_if_is_not_built_in(args, envp);
	}
	return (0);
}