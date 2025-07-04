/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:52:51 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/04 19:49:45 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	start_threads(t_sim_info *info)
{
	int	i;

	i = 0;
	info->sim_start_time = get_current_time();
	while (i < info->num_of_philos)
	{
		info->philos[i].last_eaten_time = info->sim_start_time;
		if (pthread_create(&info->philos[i].thread, NULL, \
				philo_routine, &info->philos[i]) != 0)
			return (print_error("Error: Thread creation failed.\n"));
		i++;
	}
	return (SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_sim_info	sim_info;

	if (init_simulation(&sim_info, argc, argv) != SUCCESS)
		return (FAILURE);
	if (start_threads(&sim_info) != SUCCESS)
	{
		free(sim_info.philos);
		free(sim_info.forks);
		return (FAILURE);
	}
	monitor_and_cleanup(&sim_info);
	return (SUCCESS);
}