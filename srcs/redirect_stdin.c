/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_stdin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 14:25:34 by asaadi            #+#    #+#             */
/*   Updated: 2021/01/30 09:36:30 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void redirect_to_std_in(char *name)
{
    int _fd;

    if ((_fd = open(name, O_RDONLY, S_IRUSR)) < 0)
        ft_putendl_fd(strerror(errno), 2);
}
