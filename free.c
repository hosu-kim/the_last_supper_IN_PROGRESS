/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 18:53:10 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/01 21:09:38 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_info(t_simulation_info *info)
{
	free(info->philos);
	free(info->forks);
	return (SUCCESS);
}

int	free_destroy(t_simulation_info *info)
{
	int	i;

	i = -1;
	while (++i < info->num_of_philos)
		pthread_mutex_destroy(&info->forks[i]);
	free_info(info);
	pthread_mutex_destroy(&info->print_mutex);
	pthread_mutex_destroy(&info->eat_mutex);
	pthread_mutex_destroy(&info->finish_mutex);
	return (SUCCESS);
}

void	join_free_destroy(t_simulation_info *info)
{
	int	i;

	i = -1;
	while (++i < info->num_of_philos)
		pthread_join(info->philos[i].thread, NULL);
	free_destroy(info);
}

void	destroy(t_simulation_info *info)
{
	int	i;
	int	yes;

	yes = 1;
	while (yes)
	{
		i = -1;
		info->num_of_philos = 0;
		while (++i < info->num_of_full_philos)
		{
			if (yes && is_philosopher_alive(&info->philos[i]))
				yes = 0;
		}
		usleep(10);
	}
	join_free_destory(info);
}
