/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_stdout.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 16:26:24 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/30 09:38:28 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void redirect_to_std_out(char *name)
{
    int _fd;

    if ((_fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0777)) < 0)
        ft_putendl_fd(strerror(errno), 2);
}