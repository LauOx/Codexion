/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lospina- <lospina-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/30 13:29:00 by lospina-          #+#    #+#             */
/*   Updated: 2026/06/30 14:29:42 by lospina-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codex.h"

int	main(int argc, char **argv)
{
	t_desk	desk;

	if (argc == 9)
	{
		parse_input(&desk, argv);

        //**PRUEBA TODAS LAS VARIABLES */
		printf("Variables:\n");
		printf("number_of_coders: %ld\n", desk.number_of_coders);
		printf("time_to_burnout: %lld\n", desk.time_to_burnout);
		printf("time_to_compile: %lld\n", desk.time_to_compile);
		printf("time_to_debug: %lld\n", desk.time_to_debug);
		printf("time_to_refactor: %lld\n", desk.time_to_refactor);
		printf("compiles_required: %ld\n", desk.number_of_compiles_required);
		printf("dongle_cooldown: %lld\n", desk.dongle_cooldown);
        //**PRUEBA TODAS LAS VARIABLES */

		data_init(&desk);
		start_simulation(&desk);
		free_all(&desk);
	}
	else
	{
		error_exit("Wrong input, must be 8 positive integrers", __func__);
	}
	return (0);
}