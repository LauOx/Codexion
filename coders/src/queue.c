/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospina- <lospina-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 13:29:39 by lospina-          #+#    #+#             */
/*   Updated: 2026/07/02 16:53:27 by lospina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

void	pop_waiting_list(t_queue *list)
{
	int	i;

	if (list->size == 0)
		return ;
	i = 0;
	while (i < list->size - 1)
	{
		list->array[i] = list->array[i + 1];
		i++;
	}
	list->size--;
	priority_sorter(list);
}

void	priority_sorter(t_queue *list)
{
	t_queue_item	tmp;
	int				item_zero;
	int				item_one;

	if (list->size == 2)
	{
		item_zero = list->array[0].priority;
		item_one = list->array[1].priority;
		if (item_zero < item_one)
		{
			tmp = list->array[0];
			list->array[0] = list->array[1];
			list->array[1] = tmp;
		}
	}
	else
		return ;
}
