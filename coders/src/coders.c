/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospina- <lospina-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 13:28:47 by lospina-          #+#    #+#             */
/*   Updated: 2026/07/02 20:32:32 by lospina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static bool	add_to_queue(t_coder *coder, t_dongle *dongle)
{
	t_queue_item	new_item;
	int				queue_index;

	pthread_mutex_lock(&dongle->mutex);
	new_item.coder_id = coder->id;
	if (strcmp(coder->desk->scheduler, "fifo") == 0)
		new_item.priority = get_current_time_in_ms();
	else
		new_item.priority = (coder->last_comp_time
				+ coder->desk->time_to_burnout);
	queue_index = dongle->dongle_queue.size;
	dongle->dongle_queue.array[queue_index] = new_item;
	dongle->dongle_queue.size++;
	priority_sorter(&dongle->dongle_queue);
	wait_your_turn(coder, dongle);
	if (did_simulation_ended(coder->desk) || (is_coder_burnt_out(coder)))
	{
		if (dongle->dongle_queue.size > 0)
			dongle->dongle_queue.size--;
		pthread_mutex_unlock(&dongle->mutex);
		return (false);
	}
	dongle->holding_coder_id = coder->id;
	return (true);
}

static void	wait_for_the_dongle(t_coder *coder)
{
	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
	{
		if (!add_to_queue(coder, coder->left_dongle))
			return ;
		if (!add_to_queue(coder, coder->right_dongle))
		{
			pthread_mutex_unlock(&coder->left_dongle->mutex);
			return ;
		}
	}
	else
	{
		if (!add_to_queue(coder, coder->right_dongle))
			return ;
		if (!add_to_queue(coder, coder->left_dongle))
		{
			pthread_mutex_unlock(&coder->right_dongle->mutex);
			return ;
		}
	}
}

void	get_the_dongles(t_coder *coder)
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

void	free_the_dongles(t_coder *coder)
{
	long	timestamp;

	timestamp = get_current_time_in_ms();
	coder->left_dongle->free_at = timestamp
		+ coder->left_dongle->cooldown_wait;
	coder->left_dongle->holding_coder_id = -1;
	pop_waiting_list(&coder->left_dongle->dongle_queue);
	pthread_cond_broadcast(&coder->left_dongle->cond);
	coder->right_dongle->free_at = timestamp
		+ coder->right_dongle->cooldown_wait;
	coder->right_dongle->holding_coder_id = -1;
	pop_waiting_list(&coder->right_dongle->dongle_queue);
	pthread_cond_broadcast(&coder->right_dongle->cond);
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	pthread_mutex_unlock(&coder->right_dongle->mutex);
}

void	work_in_progress(t_coder *coder)
{
	if (!did_simulation_ended(coder->desk))
	{
		print_status(coder, "is compiling");
		coder->last_comp_time = get_current_time_in_ms();
		usleep(coder->desk->time_to_compile * 1000);
		free_the_dongles(coder);
		coder->compiler_counter ++;
		coder->comp_done = coder->compiler_counter
			>= coder->desk->number_of_compiles_required;
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
