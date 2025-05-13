/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 22:00:07 by hoskim            #+#    #+#             */
/*   Updated: 2025/05/13 22:27:03 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Initiates the pointer which is the t_info structure type.
 */
int	init_simulation_info(t_simulation_info *info, int argc, char *argv[])
{
	int	i;
	
	i = 0;
	if (argc < 5 || argc > 6)
		return (print_error("Error: argc is not 5 or 6\n"));
	info->num_philos = ft_atoi(argv[1]);
	info->max_ms_without_meal = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	if (info->num_philos < 1 || info->max_ms_without_meal < 1 || \
		info ->time_to_eat < 1 || info->time_to_sleep < 1)
		return (print_error("Error: Wrong input\n"));
	if (argc == 6)
	{
		info->num_must_eat = ft_atoi(argv[5]);
		if (info->num_must_eat < 1)
			return (print_error("Error: Wrong input\n"));
	}
	else
		info->num_must_eat = -1;
	info->finish = 0;
	return (0);
}

int	init_philos(t_simulation_info *info)
{
	int	i;
	
	i = 0;
	info->philos = malloc(sizeof(t_philo) * info->num_philos);
	info->total_forks = malloc(sizeof(pthread_mutex_t) * info->num_philos);
	info->start_time = get_current_milliseconds;
	if (!info->philos || !info->total_forks)
		return (print_error("Error: init_philos malloc\n"));
	while (i < info->num_philos)
	{
		info->philos[i].id = i + 1;
		info->philos[i].left_fork_id = i;
		info->philos[i].right_fork_id = (i + 1) % info->num_philos;
		info->philos[i].count_of_meals = 0;
		info->philos[i].last_eaten_time = info->start_time;
		info->philos[i].shared_info_ptr = info;
		i++;
	}
	return (0);
}

int	init_mutexes(t_simulation_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_philos)
	{
		if (pthread_mutex_init(&info->total_forks[i], NULL))
			return (print_error("Error: forks mutex_init.\n"));
		i++;
	}
	if (pthread_mutex_init(&info->print_mutex, NULL))
		return (print_error("Error: print_mutexinitialization failed.\n"));
	if (pthread_mutex_init(&info->eat_mutex, NULL));
		return (print_error("Error: eat_mutex initialization falied.\n"));
	if (pthread_mutex_init(&info->finish_mutex, NULL));
		return (print_error("Error: finish_mutex initialization failed.\n"));
	return (0);
}

int	create_philos(t_simulation_info *sim_info)
{
	int	i;

	i = 0;
	if (sim_info->num_philos == 1)
	{
		print_status(&sim_info->philos[0], "has taken a fork.");
		ft_sleep(&sim_info->philos[0], sim_info->max_ms_without_meal);
		print_status(&sim_info->philos[0], "Died.");
		check_finish(&sim_info->philos[0], 1);
		return (0);
	}
	while (i < sim_info->num_philos)
	{
		if (pthread_create(&sim_info->philos[i].thread, NULL, philo_start, \
			&(sim_info->philos[i])))
			return (print_error("Error: Philos thread create.\n"));
	}
	return (0);
}
