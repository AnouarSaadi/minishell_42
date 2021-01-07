/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 08:54:21 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/07 16:06:12 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_cmd(char **cmd)
{
	pid_t _pid;
	int status;

	_pid = 0;
	status = 0;
	_pid = fork();
	if (_pid == -1)
	{
		ft_putendl_fd(strerror(errno), 1);
		exit(1);
	}
	else if (_pid > 0)
	{
		waitpid(_pid, &status, 0);
		kill(_pid, SIGTERM);
	}
	else
	{
		if (execve(cmd[0], cmd, NULL) == -1)
			ft_putendl_fd(strerror(errno), 1);
		exit(1);
	}
}


void do_if_is_not_built_in(char **args, char **envp)
{
	find_the_cmd_path(args, envp);
	exec_cmd(args);
	// ft_free_2dem_arr(args);
}

int	check_if_built_in(char **args, char **envp)
{
	if (!ft_strncmp(args[0], "cd", ft_strlen("cd")))
	{
		change_directory(args[1]);
		return (1);
	}
	if (!ft_strncmp(args[0], "pwd", ft_strlen("pwd")) ||
		!ft_strncmp(args[0], "PWD", ft_strlen("PWD")))
	{
		pwd_function();
		return (1);
	}
	if (!ft_strncmp(args[0], "echo", ft_strlen("echo")) ||
		!ft_strncmp(args[0], "ECHO", ft_strlen("ECHO")))
	{
		if (!ft_strncmp(args[1], "-n", ft_strlen("-n")))
			echo_function(args[2], 1, 1);
		else
			echo_function(args[1], 1, 0);
		return (1);
	}
	if (!ft_strncmp(args[0], "export", ft_strlen("export")))
	{
		envp = export_function(envp, args[1]);
		return (1);
	}
	if (!ft_strncmp(args[0], "unset", ft_strlen("unset")))
	{
		envp = unset_function(envp, args[1]);
		return (1);
	}
	if (!ft_strncmp(args[0], "env", ft_strlen("env")) ||
		!ft_strncmp(args[0], "ENV", ft_strlen("ENV")))
	{
		env_function(envp);
		return (1);
	}
	if (!ft_strncmp(args[0], "exit", ft_strlen("exit")))
	{
		if (!args[1])
			exit_function(0);
		else
			exit_function(ft_atoi(args[1]));
		return (1);
	}
	return (0);
}

int main(int ac, char **av, char **envp)
{
	int i;
	// // char *command;
	char **args;

	// args = malloc(sizeof(char *) * 3);

	// args[0] = ft_strdup("ls");
	// args[1] = ft_strdup("-la");//ft_strdup("srcs");//ft_strdup(NULL);
	// args[2] = 0;
	// // char *cmd;
	// // char **args;

	i = 0;
	ac = 0;
	av[1] = "hhhhh";
	// if (check_if_built_in(args, envp) == 0)
	// 	do_if_is_not_built_in(args, envp);
	// /* if (builtins)
	// 			echo with -n
	// 			cd
	// 			pwd
	// 			export
	// 			unset
	// 			exit
	// 			env
	// */
	// /* if not builtins  ==> PATH */
	// // command = ft_strdup("/bin/ls");
	// ft_free_2dem_arr(args);
	// // count_vars_env(envp);
	// return (0);
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
		subs_dollar(tokens_list, envp);
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
		printf("\e[0;35mseven step: join tokens type nsc\n\e[0m");
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
		cmd = fill_cmd(tokens_list);
		redir_tmp = (t_list*)cmd->redir_list;
		word_tmp = (t_list*)cmd->word_list;
		while (redir_tmp != NULL)
		{

			printf("redir type: %d ", ((t_redir*)redir_tmp->content)->type);
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
		printf("\e[1;35m	words\n\e[0m");
		while (word_tmp != NULL)
		{
			printf("%s\n", (char*)word_tmp->content);
			word_tmp = word_tmp->next;
		}
		//echo '"''"'""''"'"
		//echo $'"""''""'""''''""'"'
		printf("\e[0;33m%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\e[0m\n");
		free(line);
		i = ft_lstsize((t_list*)cmd);
		if (!(args = (char**)malloc(sizeof(char*) * (i + 1))))
			return(0);
		i = 0;
		while (cmd->word_list)
		{
			args[i] = ft_strdup(cmd->word_list->content);
			puts(args[i]);
			i++;
			cmd->word_list = cmd->word_list->next;
		}
		args[i] = NULL;
		if (check_if_built_in(args, envp) == 0)
			do_if_is_not_built_in(args, envp);
	}
	return (0);
}
