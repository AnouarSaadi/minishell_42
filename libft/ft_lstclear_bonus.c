/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/30 18:53:06 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/07 12:09:21 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*list;
	t_list	*tmp;

	list = *lst;
	if (list && del)
		while (list)
		{
			tmp = list->next;
			(*del)(list->content);
			free(list);
			list = tmp;
		}
	*lst = NULL;
}
