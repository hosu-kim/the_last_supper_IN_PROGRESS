/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:56:58 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/27 19:58:37 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Parses the command-line arguments to configure the simulation.
 * 
 * This function validates the number of arguments
 * and parses them to set up
 * the simulation parameters, such as:
 * 
 * 1. Number of philosophers (argv[1])
 * 2. Time to die (argv[2])
 * 3. Time to eat (argv[3])
 * 4. Time to sleep (argv[4])
 * 5. Optional: number of required meals (argv[5])
 * 
 * It performs basic validation to ensure all values are positive.
 * 
 * @param sim A pointer to the main simulation structure (t_simulation).
 * @param argc The count of command-line arguments.
 * @param argv The array of command-line argument strings.
 * @return Returns SUCCESS (=0) if parsing is successful, otherwise prints an
 *         error message and returns an error code.
 */
static int	parse_cmd_line_args(t_simulation *sim, int argc, char *argv[])
{
	if (argc != 5 && argc != 6)
		return (print_error("Error: Wrong number of arguments\n"));
	sim->philosopher_count = ft_atoi(argv[1]);
	sim->time_to_die = ft_atoi(argv[2]);
	sim->time_to_eat = ft_atoi(argv[3]);
	sim->time_to_sleep = ft_atoi(argv[4]);
	if (sim->philosopher_count < 1
		|| sim->time_to_die < 1
		|| sim->time_to_eat < 1
		|| sim->time_to_sleep < 1)
		return (print_error("Error: Invalid argument value.\n"));
	if (argc == 6)
	{
		sim->required_meals = ft_atoi(argv[5]);
		if (sim->required_meals < 1)
			return (print_error("Error: Invalid number of meals.\n"));
	}
	else
		sim->required_meals = -1;
	sim->simulation_ended = FALSE;
	return (SUCCESS);
}

/**
 * @brief Initializes the philosophers and fork mutexes for the simulation.
 * 
 * This function allocates memory for all the philosopher structures and the
 * corresponding fork mutexes. It then initializes each philosopher with their
 * unique ID, the indices for their left and right forks, a pointer to the
 * main simulation structure, and initial values for their meal  count and
 * last meal time.
 * 
 * @param sim A pointer to the main simulation structure (t_simulation).
 * @return Returns SUCCESS (=0) if initialization is successful, otherwise prints
 *         an error message and returns an error code
 *         for memory allocation failure.
 */
static int	setup_philosophers(t_simulation *sim)
{
	int	i;

	sim->philosophers = malloc(sizeof(t_philosopher) * sim->philosopher_count);
	sim->fork_mutexes = malloc(
			sizeof(pthread_mutex_t) * sim->philosopher_count);
	if (!sim->philosophers || !sim->fork_mutexes)
		return (print_error("Error: Memory allocation failed\n"));
	i = 0;
	while (i < sim->philosopher_count)
	{
		sim->philosophers[i].id = i + 1;
		sim->philosophers[i].left_fork_index = i;
		sim->philosophers[i].right_fork_index = \
		(i + 1) % sim->philosopher_count;
		sim->philosophers[i].simulation = sim;
		sim->philosophers[i].meals_eaten = 0;
		sim->philosophers[i].last_meal_time = 0;
		i++;
	}
	return (SUCCESS);
}

/**
 * @brief Initializes all the mutexes required for the simulation.
 * 
 * This function initializes a mutex for each fork, a mutex for controlling
 * print statements (to prevent garbled output), and a mutex for protecting
 * shared data within the simulation structure.
 * 
 * @param sim A pointer to the t_simulation struct which holds all simulation
 *            data, including the mutexes to be initialized.
 * @return Returns SUCCESS (=0) if all mutexes are created successfully.
 *         If any mutex initialization fails, it returns an error code and prints
 *         a corresponding error message to standard error.
 */
static int	initialize_mutexes(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->philosopher_count)
	{
		if (pthread_mutex_init(&sim->fork_mutexes[i], NULL) != SUCCESS)
			return (print_error("Error: Fork mutex initialization failed.\n"));
		i++;
	}
	if (pthread_mutex_init(&sim->mutex_for_printing, NULL) != SUCCESS)
		return (print_error("Error: Print mutex initialization failed.\n"));
	if (pthread_mutex_init(&sim->data_mutex, NULL) != SUCCESS)
		return (print_error("Error: Data mutex initialization failed.\n"));
	return (SUCCESS);
}

/**
 * @brief Initializes the entire simulation structrue.
 * 
 * This function serves as the main entry point for initialization.
 * It calls helper functions in sequence to:
 * 1. Parse the command-line arguments.
 * 2. Set up the philosopher structures and fork mutexes.
 * 3. Initialize all necessary mutexes for synchronization.
 * If any of these steps fail, the function will immediately abort the
 * initialization process and return a failure status.
 * 
 * @param sim A pointer to the main simulation structure (t_simulation) that
 *            will be populated with the simulation's configuration.
 * @param argc The count of command-line arguments passed to the program.
 * @param argv The array of command-line argument strings.
 * @return Returns SUCCESS (=0) if the entire simulation is
 *         initialized successfully. Otherwise, it returns FAILURE (=1).
 */
int	initialize_simulation(t_simulation *sim, int argc, char *argv[])
{
	if (parse_cmd_line_args(sim, argc, argv) != SUCCESS)
		return (FAILURE);
	if (setup_philosophers(sim) != SUCCESS)
		return (FAILURE);
	if (initialize_mutexes(sim) != SUCCESS)
		return (FAILURE);
	return (SUCCESS);
}
