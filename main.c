/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:52:51 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/27 23:22:56 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Launches all philosopher threads and sets the simulation start time.
 * 
 * This function initializes the simulation by recording the start time.
 * It then iterates through each philsopher,
 * setting their initial `last_meal_time` to the simulation's start time.
 * A new thread is created for each philosopher, which will execute
 * the `philosopher_lifecycle` function.
 * 
 * @param sim A pointer to the simulation structure,
 *            containing all simulation data.
 * @return Returns `SUCCESS` if all threads are created successfully. Otherwise,
 *         it prints an error message and returns an error code.
 * 
 */
static int	launch_philosopher_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	sim->sim_start_time = get_current_time_ms();
	while (i < sim->philosopher_count)
	{
		sim->philosophers[i].last_meal_time = sim->sim_start_time;
		if (pthread_create(&sim->philosophers[i].thread, NULL, \
				philosopher_lifecycle, &sim->philosophers[i]) != SUCCESS)
			return (
				print_error("Error: Failed to create philosopher thread.\n"));
		i++;
	}
	return (SUCCESS);
}

/**
 * @brief The main entry point for the Dining Philosophers simulation.
 * 
 * This function orchestrates the entire simulation.
 * It performs the following steps:
 * 1. Initializes the simulation state, including philosophers, forks, and rules,
 *    based on the command-line arguments provided.
 * 2. Launches the threads for each philosopher, starting their lifecycles.
 * 3. Enters a monitoring loop that checks the state of the simulation
 *    (e.g., for philosopher deaths or completion of the simulation) and performs
 *    the final cleanup of resources.
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return Returns `SUCCESS` if the simulation completes without errors,
 *         otherwise returns `FAILURE`.
 */
int	main(int argc, char *argv[])
{
	t_simulation	simulation;

	if (initialize_simulation(&simulation, argc, argv) != SUCCESS)
		return (FAILURE);
	if (launch_philosopher_threads(&simulation) != SUCCESS)
	{
		free(simulation.philosophers);
		free(simulation.mutex_for_fork);
		return (FAILURE);
	}
	monitor_simulation_and_cleanup(&simulation);
	return (SUCCESS);
}
