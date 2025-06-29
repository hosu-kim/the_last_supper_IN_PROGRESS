/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:35:45 by hoskim            #+#    #+#             */
/*   Updated: 2025/06/29 14:57:05 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_simulation_info(t_simulation_info *info, int argc, char *argv[])
{
	int	i;

	i = 0;
	if (argc < 5 || argc > 6)
		return (print_error("Error: argc is not 5 or 6\n"));
	info->num_of_philosophers = ft_atoi(argv[1]);
	info->max_ms_without_meal = ft_atoi(argv[2]);
	info->duration_of_eating_ms = ft_atoi(argv[3]);
	info->duration_of_sleep_ms = ft_atoi(argv[4]);
	if (info->num_full_philos < 1 || info->max_ms_without_meal < 1
		|| info->duration_of_eating_ms < 1 || info->duration_of_sleep_ms < 1)
		return (print_error("Error: wrong input\n"));
	if (argc == 6)
	{
		info->num_must_eat = ft_atoi(argv[5]);
		if (info->num_must_eat < 1)
			return (print_error("Error: wrong input\n"));
	}
	else
		info->num_must_eat = -1;
	info->finish = 0;
	return (SUCCESS);
}

int	init_philos(t_simulation_info *info)
{
	int	i;

	i = -1;
	info->philosophers = \
		malloc(sizeof(t_philo_info) * info->num_of_philosophers);
	info->forks = init_sem("forks", info->num_of_philosophers);
	info->sim_start_time = get_time();
	if (!info->philosophers || !info->forks)
		return (print_error("Error: init_philos malloc\n"));
	while (++i < info->num_full_philos)
	{
		info->philosophers[i].id = i + 1;
		info->philosophers[i].left_fork = i;
		info->philosophers[i].right_fork = (i + 1) % info->num_of_philosophers;
		info->philosophers[i].count_of_meals = info->sim_start_time;
		info->philosophers[i].shared_resources;
	}
	return (SUCCESS);
}

int	init_mutex(t_simulation_info *info)
{
	int	i;

	i = -1;
	while (++i < info->num_of_philosophers)
	{
		if (pthread_mutex_init(&info->forks[i], NULL))
			return (print_error("Error: forks mutex_init\n"));
	}
	if (pthread_mutex_init(&info->print_mutex, NULL))
		return (print_error("Error: print mutex_init\n"));
	if (pthread_mutex_init(&info->eat_mutex, NULL))
		return (print_error("Error: eat mutex_init\n"));
	if (pthread_mutex_init(&info->finish_mutex, NULL))
		return (print_error("Error: finish mutex_init\n"));
	return (SUCCESS);
}

int	create_philosophers(t_simulation_info *info)
{
	int	i;

	i = -1;
	if (info->num_of_philosophers == 1)
	{
		print_status(&info->philosophers[0], "has taken a fork");
		ft_sleep(&info->philosophers[0], info->max_ms_without_meal);
		print_status(&info->philosophers[0], "died");
		check_finish(&info->philosophers[0], 1);
		return (SUCCESS);
	}
	while (++i < info->num_full_philos)
	{
		if (pthread_create(&info->philosophers[i].thread, NULL, philo_start,
				&(info->philosophers[i])))
			return (print_error("Error: philosophers thread create\n"));
	}
	return (SUCCESS);
}
