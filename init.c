/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:56:58 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/05 19:48:25 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	parse_arguments(t_simulation *sim, int argc, char *argv[])
{
	if (argc != 5 && argc != 6)
		return (print_error("Error: Wrong number of arguments\n"));
	sim->philosopher_count = ft_atoi(argv[1]);
	sim->time_to_die = ft_atoi(argv[2]);
	sim->time_to_eat = ft_atoi(argv[3]);
	sim->time_to_sleep = ft_atoi(argv[4]);
	if (sim->philosopher_count < 1 || sim->time_to_die < 1
		|| sim->time_to_eat < 1 || sim->time_to_sleep < 1)
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

static int	setup_philosophers(t_simulation *sim)
{
	int	i;

	sim->philosophers = malloc(sizeof(t_philosopher) * sim->philosopher_count);
	sim->fork_mutexes = malloc(sizeof(pthread_mutex_t) * sim->philosopher_count);
	if (!sim->philosophers || !sim->fork_mutexes)
		return (print_error("Error: Memory allocation failed\n"));
	i = 0;
	while (i < sim->philosopher_count)
	{
		sim->philosophers[i].id = i + 1;
		sim->philosophers[i].left_fork_index = i;
		sim->philosophers[i].right_fork_index = (i + 1) % sim->philosopher_count;
		sim->philosophers[i].simulation = sim;
		sim->philosophers[i].meals_eaten = 0;
		sim->philosophers[i].last_meal_time = 0;
		i++;
	}
	return (SUCCESS);
}

static int	initialize_mutexes(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->philosopher_count)
	{
		if (pthread_mutex_init(&sim->fork_mutexes[i], NULL) != 0)
			return (print_error("Error: Fork mutex initialization failed.\n"));
		i++;
	}
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (print_error("Error: Print mutex initialization failed.\n"));
	if (pthread_mutex_init(&sim->data_mutex, NULL) != 0)
		return (print_error("Error: Data mutex initialization failed.\n"));
	return (SUCCESS);
}

int	initialize_simulation(t_simulation *sim, int argc, char *argv[])
{
	if (parse_arguments(sim, argc, argv) != SUCCESS)
		return (FAILURE);
	if (setup_philosophers(sim) != SUCCESS)
		return (FAILURE);
	if (initialize_mutexes(sim) != SUCCESS)
		return (FAILURE);
	return (SUCCESS);
}
