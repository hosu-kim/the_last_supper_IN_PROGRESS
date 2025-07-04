/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:56:58 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/04 19:51:15 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_args(t_sim_info *info, int argc, char *argv[])
{
	if (argc != 5 && argc != 6)
		return (print_error("Error: Wrong number of arguments\n"));
	info->num_of_philos = ft_atoi(argv[1]);
	info->max_time_without_meal = ft_atoi(argv[2]);
	info->eating_duration = ft_atoi(argv[3]);
	info->sleeping_duration = ft_atoi(argv[4]);
	if (info->num_of_philos < 1 || info->max_time_without_meal < 1
		|| info->eating_duration < 1 || info->sleeping_duration < 1)
		return (print_error("Error: Invalid argument value.\n"));
	if (argc == 6)
	{
		info->num_must_eat = ft_atoi(argv[5]);
		if (info->num_must_eat < 1)
			return (print_error("Error: Invalid number of meals.\n"));
	}
	else
		info->num_must_eat = -1;
	info->sim_finished = 0;
	info->num_of_full_philos = 0;
	return (SUCCESS);
}

static int	init_philos(t_sim_info *info)
{
	int	i;

	info->philos = malloc(sizeof(t_philo_info) * info->num_of_philos);
	info->forks = malloc(sizeof(pthread_mutex_t) * info->num_of_philos);
	if (!info->philos || !info->forks)
		return (print_error("Error: Malloc failed\n"));
	i = 0;
	while (i < info->num_of_philos)
	{
		info->philos[i].philosopher_id = i + 1;
		info->philos[i].left_fork_id = i;
		info->philos[i].right_fork_id = (i + 1) % info->num_of_philos;
		info->philos[i].sim_info = info;
		info->philos[i].meal_count = 0;
		info->philos[i].last_eaten_time = 0;
		i++;
	}
	return (SUCCESS);
}

static int	init_mutexes(t_sim_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_of_philos)
	{
		if (pthread_mutex_init(&info->forks[i], NULL) != 0)
			return (print_error("Error: Mutex init failed.\n"));
		i++;
	}
	if (pthread_mutex_init(&info->print_mutex, NULL) != 0)
		return (print_error("Error: Mutex init failed.\n"));
	if (pthread_mutex_init(&info->data_mutex, NULL) != 0)
		return (print_error("Error: Mutex init failed.\n"));
	return (SUCCESS);
}

int	init_simulation(t_sim_info *info, int argc, char *argv[])
{
	if (init_args(info, argc, argv) != SUCCESS)
		return (FAILURE);
	if (init_philos(info) != SUCCESS)
		return (FAILURE);
	if (init_mutexes(info) != SUCCESS)
		return (FAILURE);
	return (SUCCESS);
}
