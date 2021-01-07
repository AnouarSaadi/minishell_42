/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 08:54:21 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/07 12:59:32 by asaadi           ###   ########.fr       */
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

void if_is_not_built_in(char **args, char **envp)
{
	find_the_cmd_path(args, envp);
	exec_cmd(args);
	// ft_free_2dem_arr(args);
}

int main(int ac, char **av, char **envp)
{
	int i;
	// char *command;
	char **args;

	args = malloc(sizeof(char *) * 3);

	args[0] = ft_strdup("ls");
	args[1] = 0;//ft_strdup("srcs");//ft_strdup(NULL);
	args[2] = 0;
	// char *cmd;
	// char **args;

	i = 0;
	ac = 0;
	av[1] = "hhhhh";
	/* if (builtins)
				echo with -n
				cd
				pwd
				export
				unset
				exit
				env
	*/
	/* if not builtins  ==> PATH */
	// command = ft_strdup("/bin/ls");
	if_is_not_built_in(args, envp);
	ft_free_2dem_arr(args);
	count_vars_env(envp);
	return (0);
}
