/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_stdout.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 16:26:24 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/28 14:25:55 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void redirect_to_std_out(char **args, char **envp)
{
    int _fd;

    if((_fd = open(args[0], O_CREAT | O_RDWR)) < 0)
        ft_putendl_fd(strerror(errno), 2);
    dup2(_fd, 1);
}