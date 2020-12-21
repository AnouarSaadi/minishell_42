/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 19:35:10 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/08 12:47:41 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*s_list;
	t_list	*tmp;

	if (!lst || !f || !del)
		return (NULL);
	s_list = NULL;
	while (lst)
	{
		tmp = ft_lstnew((*f)(lst->content));
		if (!tmp && del)
		{
			ft_lstclear(&s_list, del);
			return (NULL);
		}
		ft_lstadd_back(&s_list, tmp);
		lst = lst->next;
	}
	return (s_list);
}
