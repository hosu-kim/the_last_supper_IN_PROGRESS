/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:52:51 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/05 15:48:33 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	launch_philosopher_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	sim->start_time = get_current_time_ms();
	while (i < sim->philosopher_count)
	{
		sim->philosophers[i].last_meal_time = sim->start_time;
		if (pthread_create(&sim->philosophers[i].thread, NULL, \
				philosopher_lifecycle, &sim->philosophers[i]) != 0)
			return (print_error("Error: Failed to create philosopher thread.\n"));
		i++;
	}
	return (SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_simulation	simulation;

	if (initialize_simulation(&simulation, argc, argv) != SUCCESS)
		return (FAILURE);
	if (launch_philosopher_threads(&simulation) != SUCCESS)
	{
		free(simulation.philosophers);
		free(simulation.fork_mutexes);
		return (FAILURE);
	}
	monitor_simulation_and_cleanup(&simulation);
	return (SUCCESS);
}
