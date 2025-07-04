/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:31:27 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/04 20:10:12 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_death(t_sim_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_of_philos)
	{
		if ((get_current_time() - info->philos[i].last_eaten_time)
		> info->max_time_without_meal)
		{
			info->sim_finished = YES;
			print_status(&info->philos[i], "died", YES);
			return (YES);
		}
		i++;
	}
	return (NO);
}

static int	check_all_ate(t_sim_info *info)
{
	int	i;
	int	full_philos_count;

	if (info->num_must_eat == -1)
		return (NO);
	i = 0;
	full_philos_count = 0;
	while (i < info->num_of_philos)
	{
		if (info->philos[i].meal_count >= info->num_must_eat)
			full_philos_count++;
		i++;
	}
	if (full_philos_count >= info->num_of_philos)
	{
		info->sim_finished = YES;
		return (YES);
	}
	return (NO);
}

static int	check_philosopher_status(t_sim_info *info)
{
	pthread_mutex_lock(&info->data_mutex);
	if (check_death(info) == YES)
	{
		pthread_mutex_unlock(&info->data_mutex);
		return (YES);
	}
	if (check_all_ate(info) == YES)
	{
		pthread_mutex_unlock(&info->data_mutex);
		return (YES);
	}
	pthread_mutex_unlock(&info->data_mutex);
	return (NO);
}

static void	destroy_resources(t_sim_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_of_philos)
	{
		pthread_join(info->philos[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < info->num_of_philos)
	{
		pthread_mutex_destroy(&info->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&info->print_mutex);
	pthread_mutex_destroy(&info->data_mutex);
	free(info->philos);
	free(info->forks);
}

void	monitor_and_cleanup(t_sim_info *info)
{
	while (1)
	{
		if (check_philosopher_status(info) == YES)
			break ;
		usleep(1000);
	}
	destroy_resources(info);
}
