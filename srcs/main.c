/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 08:54:21 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/06 18:31:04 by asaadi           ###   ########.fr       */
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
		strerror(errno);
	else if (_pid > 0)
	{
		waitpid(_pid, &status, 0);
		kill(_pid, SIGTERM);
	}
	else
	{
		if (execve(cmd[0], cmd, NULL) == -1)
			strerror(errno);
		exit(1);
	}
}

void if_not_built_in(char *command, char **envp)
{
	char *cmd;
	char *path;
	char **args;
	char **path_split;
	int i;

	args = (char **)malloc(sizeof(char *) * 2);
	i = 0;
	/* find the command path */
	// if ((path = check_var_env(envp, "PATH")) != NULL)
	// {
	// 	path_split = ft_split(path, ':');
	// 	while (path_split[i])
	// 	{
	// 		// puts(path_split[i]);
	// 		ft_strlcat(path_split[i], command, ft_strlen(path_split[i] + ft_strlen(command) + 1));
	// 		i++;
	// 	}
	// }
	if (ft_strncmp("/bin/", command, 5) != 0)
	{
		if (!(cmd = (char *)malloc(sizeof(char) * ft_strlen("/bin/") + ft_strlen(command) + 1)))
		{
			ft_putendl_fd("allocation error", 1);
			exit(1);
		}
		ft_strlcpy(cmd, "/bin/", ft_strlen("/bin/") + 1);
		ft_strlcat(cmd, command, ft_strlen(cmd) + ft_strlen(command) + 1);
	}
	else
	{
		if (!(cmd = malloc(sizeof(char) * ft_strlen(command) + 1)))
		{
			ft_putendl_fd("allocation error", 1);
			exit(1);
		}
		cmd = ft_strdup(command);
	}
	args[0] = cmd;
	args[1] = 0;
	exec_cmd(args);
	free(cmd);
}

int main(int ac, char **av, char **envp)
{
	int i;
	char *command;
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
	command = ft_strdup("ls");
	if_not_built_in(command, envp);
	count_vars_env(envp);
	return (0);
}
