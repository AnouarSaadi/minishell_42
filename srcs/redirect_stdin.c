/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_stdin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 14:25:34 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/29 14:54:57 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void redirect_to_std_in(char **args, char *name, char ***envp)
{
    int _fd;
    pid_t p;
    int i;


    p = fork();
    if (p == 0)
    {
        if ((_fd = open(name, O_RDONLY, S_IRUSR)) < 0)
            ft_putendl_fd(strerror(errno), 2);
        dup2(_fd, 0);
    puts(name);
        close(_fd);
        check_if_built_in(args, envp, &i);
        if (i == 0)
            do_if_is_not_built_in(args, *envp);
        exit(0);
    }
    wait(0);
}
