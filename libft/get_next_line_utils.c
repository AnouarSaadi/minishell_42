/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-mak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 14:14:20 by abel-mak          #+#    #+#             */
/*   Updated: 2020/12/29 10:02:45 by abel-mak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_glist		*ladd(t_glist **head, t_glist *new)
{
	if (new)
	{
		new->next = *head;
		return (new);
	}
	return (NULL);
}
