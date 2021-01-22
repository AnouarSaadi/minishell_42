/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 08:54:21 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/22 15:45:30 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **get_args_execve(char *binarypath, char *cmd)
{
	char **args;

	if (!(args = (char **)malloc(sizeof(char*) * 4)))
		return(NULL);
	args[0] = ft_strdup(binarypath);
	args[1] = ft_strdup("-c");
	args[2] = ft_strdup(cmd);
	args[3] = NULL;
	return (args);
}

void exec_cmd(char *cmd, char **envp)
{
	pid_t _pid;
	int status;
	char *binarypath;
	char **args;

	_pid = 0;
	status = 0;
	_pid = fork();
	if (_pid == -1)
	{
		ft_putendl_fd(strerror(errno), 1);
		exit(EXIT_FAILURE);
	}
	else if (_pid > 0)
	{
		waitpid(_pid, &status, 0);
		kill(_pid, SIGTERM);
	}
	else
	{
	    binarypath = "/bin/bash";
        args = get_args_execve(binarypath, cmd);
        if (execve(binarypath, args, envp) == -1)
            ft_putendl_fd(strerror(errno),1);
		ft_free_2dem_arr(args);
		exit(EXIT_FAILURE);
	}
}


void do_if_is_not_built_in(char **args, char **envp)
{
	int i;
	char *arg;
	(void)envp;

	i = 0;
	while (args[i])
	{
		if (i == 0)
			arg = ft_strjoin(args[i], "");
		else
		{
			arg = ft_strjoin(arg, " ");
			arg = ft_strjoin(arg, args[i]);
		}
		i++;
	}
	exec_cmd(arg, envp);
	free(arg);
	ft_free_2dem_arr(args);
}

char **check_if_built_in(char **args, char **envp, int *i)
{
	*i = 0;

	if (!ft_strncmp(args[0], "cd", ft_strlen("cd")))
	{
		change_directory(args[1], envp);
		*i = 1;

	}
	if (!ft_strncmp(args[0], "pwd", ft_strlen("pwd")) ||
		!ft_strncmp(args[0], "PWD", ft_strlen("PWD")))
	{
		pwd_function();
		*i = 1;
	}
	if (!ft_strncmp(args[0], "echo", ft_strlen("echo")) ||
		!ft_strncmp(args[0], "ECHO", ft_strlen("ECHO")))
	{
		if (args[1] && !ft_strncmp(args[1], "-n", ft_strlen("-n")))
			echo_function(args[2], 1, 1);
		else
			echo_function(args[1], 1, 0);
		*i = 1;
	}
	if (!ft_strncmp(args[0], "export", ft_strlen("export")))
	{
		if (args[1])
			export_function(envp, args[1]);
		else
			sort_print_envp_alpha(envp);
		*i = 1;
	}
	if (!ft_strncmp(args[0], "unset", ft_strlen("unset")))
	{
		envp = unset_function(envp, args[1]);
		*i = 1;
	}
	if (!ft_strncmp(args[0], "env", ft_strlen("env")) ||
		!ft_strncmp(args[0], "ENV", ft_strlen("ENV")))
	{
		env_function(envp);
		*i = 1;
	}
	if (!ft_strncmp(args[0], "exit", ft_strlen("exit")))
	{
		if (!args[1])
			exit_function(0);
		else
			exit_function(ft_atoi(args[1]));
		*i = 1;
	}
	// int i = 0;
	// while (*envp[i])
	// {
	// 	puts(*envp[i]);
	// 	i++;
	// }
	return (envp);
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
	// // char *command;
	char **args;
	char **envp;

	// args = malloc(sizeof(char *) * 3);

	// args[0] = ft_strdup("ls");
	// args[1] = ft_strdup("-la");//ft_strdup("srcs");//ft_strdup(NULL);
	// args[2] = 0;
	// // char *cmd;
	// // char **args;
	envp = envp_cpy(env);
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
	// t_list *tmp;
	// t_token *token;
	int r;
	// t_list *redir_tmp;
	// t_list *word_tmp;
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
		subs_dollar(tokens_list, env);
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
		// redir_tmp = (t_list*)cmd->redir_list;
		// word_tmp = (t_list*)cmd->word_list;
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
		//echo '"''"'""''"'"
		//echo $'"""''""'""''''""'"'
		// printf("\e[0;33m%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\e[0m\n");
		free(line);
		i = 0;
		i = ft_lstsize(cmd->word_list);
		if (!(args = (char**)malloc(sizeof(char*) * (i + 1))))
			return(0);
		i = 0;
		while (cmd->word_list)
		{
			args[i] = ft_strdup(cmd->word_list->content);
			i++;
			cmd->word_list = cmd->word_list->next;
		}
		args[i] = NULL;
		envp = check_if_built_in(args, envp, &i);
		if (i == 0)
			do_if_is_not_built_in(args, envp);
	}
	return (0);
}
