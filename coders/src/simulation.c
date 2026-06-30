/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospina- <lospina-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 13:29:49 by lospina-          #+#    #+#             */
/*   Updated: 2026/06/30 17:11:01 by lospina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codex.h"

static void	*run_coder(void *argv)
{
	t_coder	*coder;

	coder = (t_coder *)argv; // casteamos para que lo que entró se convierta en coder
	while (!did_simulation_ended(coder->desk))
	{
		assign_the_dongles(coder);

		if (did_simulation_ended(coder->desk))
			break ;
		work_in_progress(coder);
	}

	return (NULL);
}

static void	call_it_a_day(t_desk *desk, long coder_done)
{
	pthread_mutex_lock(&desk->sim_mutex);
	desk->end_simulation = coder_done == desk->number_of_coders;
	pthread_mutex_unlock(&desk->sim_mutex);
}

void	*monitor(void *argv)
{
	int		i;
	long	coder_done;
	t_desk	*desk;

	desk = (t_desk *)argv;
	while (!did_simulation_ended(desk))
	{
		i = -1;
		coder_done = 0;
		while (++i < desk->number_of_coders)
		{
			if (is_coder_burnt_out(&desk->coders[i]))
				return (NULL);
			if (did_simulation_ended(desk))
				break ;
			if (desk->coders[i].compiler_counter
				>= desk->number_of_compiles_required)
				coder_done ++;
		}
		if (did_simulation_ended(desk))
			return (NULL);
		call_it_a_day(desk, coder_done);
		usleep(500);
	}
	return (NULL);
}


void	start_simulation(t_desk *desk)
{
	int	i;

	i = -1;
	if (desk->number_of_compiles_required == 0)
		return ;
	desk->start_time = get_current_time_in_ms();

	// inicialización de timepo de ultima compilación
	while (++i < desk->number_of_coders)
		desk->coders[i].last_comp_time = desk->start_time;

	i = -1;
	while (++i < desk->number_of_coders)
		pthread_create(&desk->coders[i].thread_id, NULL,
			run_coder, &desk->coders[i]);

	pthread_create(&desk->start_simulation, NULL, monitor, desk);

	i = -1;
	while (++i < desk->number_of_coders)
		pthread_join(desk->coders[i].thread_id, NULL);
	pthread_join(desk->start_simulation, NULL);
}