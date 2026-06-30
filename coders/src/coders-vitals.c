/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders-vitals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospina- <lospina-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 13:28:21 by lospina-          #+#    #+#             */
/*   Updated: 2026/06/30 17:10:42 by lospina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static void	somebody_died(t_coder *coder)
{
	pthread_mutex_lock(&coder->desk->log_mutex);
	if (!coder->desk->end_simulation)
	{
		printf("%ld %d burned out\n", get_current_time_in_ms()
			- coder->desk->start_time, coder->id);
	}
	pthread_mutex_unlock(&coder->desk->log_mutex);
	pthread_mutex_lock(&coder->desk->sim_mutex);
	coder->desk->end_simulation = true;
	pthread_mutex_unlock(&coder->desk->sim_mutex);
	return ;
}

bool	is_coder_burnt_out(t_coder *coder)
{
	if ((get_current_time_in_ms() - coder->last_comp_time)
		> coder->desk->time_to_burnout)
	{
		somebody_died(coder);
		return (true);
	}
	return (false);
}

bool	did_simulation_ended(t_desk *desk)
{
	bool	stop;

	pthread_mutex_lock(&desk->sim_mutex);
	stop = desk->end_simulation;
	pthread_mutex_unlock(&desk->sim_mutex);

	return (stop);
}

void	wait_your_turn(t_coder *coder, t_dongle *dongle)
{
	long		timestamp;

	timestamp = get_current_time_in_ms();
	while ((dongle->dongle_queue.array[0].coder_id != coder->id)
		|| (timestamp < dongle->free_at))
	{
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
		timestamp = get_current_time_in_ms();
	}
}
