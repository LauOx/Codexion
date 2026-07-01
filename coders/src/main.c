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