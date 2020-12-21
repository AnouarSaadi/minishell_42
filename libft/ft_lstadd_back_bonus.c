/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 11:53:39 by asaadi            #+#    #+#             */
/*   Updated: 2019/11/03 14:37:18 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list *last;

	if (alst)
	{
		if (!(*alst))
		{
			*alst = new;
			return ;
		}
		last = ft_lstlast(*alst);
		last->next = new;
	}
}
