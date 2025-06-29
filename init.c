/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:35:45 by hoskim            #+#    #+#             */
/*   Updated: 2025/06/29 18:55:47 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_simulation_info(t_simulation_info *info, int argc, char *argv[])
{
	int	i;

	i = 0;
	if (argc < 5 || 6 > argc)
		return (print_error("Error: The number of argc is not 5 or 6\n"));
	info->num_of_philos = ft_atoi(argv[1]);
	info->max_time_without_meal = ft_atoi(argv[2]);
	info->eating_duration = ft_atoi(argv[3]);
	info->sleeping_duration = ft_atoi(argv[4]);
	if (info->num_of_philos < 1 || info->max_time_without_meal < 1
		|| info->eating_duration < 1 || info->sleeping_duration < 1)
		return (print_error("Error: wrong input\n"));
	if (argc == 6)
	{
		info->num_of_philos_must_eat = ft_atoi(argv[5]);
		if (info->num_of_philos_must_eat < 1)
			return (print_error("Error: wrong input\n"));
	}
	else
		info->num_of_philos_must_eat = -1;
	info->is_sim_finished = 0;
	return (SUCCESS);
}

int	init_philo_info(t_simulation_info *sim_info)
{
	int	i;

	i = 0;
	sim_info->philos = malloc(sizeof(t_philo_info) * sim_info->num_of_philos);
	sim_info->forks = malloc(sizeof(pthread_mutex_t) * sim_info->num_of_philos);
	if (!sim_info->philos || !sim_info->forks)
		return (print_error("Error: init_philos malloc.\n"));
	sim_info->sim_start_time = get_current_time();
	while (i < sim_info->num_of_philos)
	{
		sim_info->philos[i].philosopher_id = i + 1;
		sim_info->philos[i].left_fork_id = i;
		sim_info->philos[i].right_fork_id = \
			(i + 1) % sim_info->num_of_philos;
		sim_info->philos[i].last_meal_time = sim_info->sim_start_time;
		sim_info->philos[i].shared_resources = sim_info;
		i++;
	}
	return (SUCCESS);
}

int	init_mutex(t_simulation_info *sim_info)
{
	int	i;

	i = -1;
	while (++i < sim_info->num_of_philos)
	{
		if (pthread_mutex_init(&sim_info->forks[i], NULL))
			return (print_error("Error: forks mutex_init\n"));
	}
	if (pthread_mutex_init(&sim_info->print_mutex, NULL))
		return (print_error("Error: print mutex_init\n"));
	if (pthread_mutex_init(&sim_info->eat_mutex, NULL))
		return (print_error("Error: eat mutex_init\n"));
	if (pthread_mutex_init(&sim_info->finish_mutex, NULL))
		return (print_error("Error: finish mutex_init\n"));
	return (SUCCESS);
}

int	create_philosophers(t_simulation_info *sim_info)
{
	int	i;

	i = -1;
	if (sim_info->num_of_philos == 1)
	{
		print_status(&sim_info->philos[0], "has taken a fork");
		ft_sleep(&sim_info->philos[0], sim_info->max_time_without_meal);
		print_status(&sim_info->philos[0], "died");
		check_finish(&sim_info->philos[0], 1);
		return (SUCCESS);
	}
	while (++i < sim_info->num_of_philos)
	{
		if (pthread_create(&sim_info->philos[i].thread, NULL, philo_start,
				&(sim_info->philos[i])))
			return (print_error("Error: philosophers thread create\n"));
	}
	return (SUCCESS);
}
