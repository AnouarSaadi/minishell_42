/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_stdout.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 16:26:24 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/29 12:33:16 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void redirect_to_std_out(char **args, char *name, char ***envp)
{
    int _fd;
    int i;
    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        if ((_fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0777)) < 0)
            ft_putendl_fd(strerror(errno), 2);
        if(dup2(_fd, 1) < 0)
            ft_putendl_fd(strerror(errno), 2);
        close(_fd);
        check_if_built_in(args, envp, &i);
        if (i == 0)
            do_if_is_not_built_in(args, *envp);
        exit(0);
    }
    wait(0);
}