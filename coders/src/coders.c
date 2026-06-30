/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospina- <lospina-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 13:28:47 by lospina-          #+#    #+#             */
/*   Updated: 2026/06/30 17:10:29 by lospina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static void	add_to_queue(t_desk *desk, t_queue *list, t_coder coder)
{
	int				queue_index;
	t_queue_item	new_item;

	new_item.coder_id = coder.id;
	if (strcmp(desk->scheduler, "fifo") == 0)
		new_item.priority = (long long)time(NULL);
	else
	{
		new_item.priority = (coder.last_comp_time + desk->time_to_burnout);
	}
	queue_index = list->size;
	list->array[queue_index] = new_item;
	list->size ++;
	priority_sorter(list);
}

static void	wait_for_the_dongle(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->left_dongle;
	right = coder->right_dongle;

	pthread_mutex_lock(&left->mutex);
	add_to_queue(coder->desk, &left->dongle_queue, *coder);
	wait_your_turn(coder, left);
	if (!is_coder_burnt_out(coder))
		left->holding_coder_id = coder->id;
	pthread_mutex_unlock(&left->mutex);

	if (!coder->desk->end_simulation)
	{
		pthread_mutex_lock(&right->mutex);
		add_to_queue(coder->desk, &right->dongle_queue, *coder);
		wait_your_turn(coder, right);
		if (!is_coder_burnt_out(coder))
			right->holding_coder_id = coder->id;

		pthread_mutex_unlock(&right->mutex);
	}
}

static void	free_the_dongles(t_coder *coder)
{
	long	timestamp;

	timestamp = get_current_time_in_ms();

	pthread_mutex_lock(&coder->left_dongle->mutex);
	coder->left_dongle->free_at = timestamp
		+ coder->left_dongle->cooldown_wait;
	coder->left_dongle->holding_coder_id = -1;
	pop_waiting_list(&coder->left_dongle->dongle_queue);
	pthread_cond_broadcast(&coder->left_dongle->cond);
	pthread_mutex_unlock(&coder->left_dongle->mutex);

	pthread_mutex_lock(&coder->right_dongle->mutex);
	coder->right_dongle->free_at = timestamp
		+ coder->right_dongle->cooldown_wait;
	coder->right_dongle->holding_coder_id = -1;
	pop_waiting_list(&coder->right_dongle->dongle_queue);
	pthread_cond_broadcast(&coder->right_dongle->cond);
	pthread_mutex_unlock(&coder->right_dongle->mutex);
}

void	assign_the_dongles(t_coder *coder)
{
	if (!did_simulation_ended(coder->desk))
	{
		wait_for_the_dongle(coder);
		if (did_simulation_ended(coder->desk))
			return ;
		print_status(coder, "has taken a dongle");
		print_status(coder, "has taken a dongle");
	}
}

void	work_in_progress(t_coder *coder)
{
	if (!did_simulation_ended(coder->desk))
	{
		print_status(coder, "is compiling");
		usleep(coder->desk->time_to_compile * 1000);
		coder->last_comp_time = get_current_time_in_ms();
		free_the_dongles(coder);
		coder->compiler_counter ++;
	}

	if (!did_simulation_ended(coder->desk))
	{
		print_status(coder, "is debbuging");
		usleep(coder->desk->time_to_debug * 1000);
	}

	if (!did_simulation_ended(coder->desk))
	{
		print_status(coder, "is refactoring");
		usleep(coder->desk->time_to_refactor * 1000);
	}

}