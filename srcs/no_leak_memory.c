/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_leak_memory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaadi <asaadi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/07 12:53:08 by asaadi            #+#    #+#             */
/*   Updated: 2021/02/02 18:01:43 by asaadi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_arr(void *array)
{
	if (array)
		free(array);
	array = NULL;
}


void	ft_free_2dem_arr(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
}