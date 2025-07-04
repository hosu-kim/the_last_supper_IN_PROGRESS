/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:22:39 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/04 19:57:00 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_eat(t_philo_info *philo)
{
	t_sim_info	*info;
	
	info = philo->sim_info;
	pthread_mutex_lock(&info->forks[philo->left_fork_id]);
	print_status(philo, "has taken a fork", NO);
	if (info->num_of_philos == 1)
	{
		ft_sleep(philo, info->max_time_without_meal * 2);
		pthread_mutex_unlock(&info->forks[philo->left_fork_id]);
		return ;
	}
	pthread_mutex_lock(&info->forks[philo->right_fork_id]);
	print_status(philo, "has taken a fork", NO);
	print_status(philo, "is eating", NO);
	pthread_mutex_lock(&info->data_mutex);
	philo->last_eaten_time = get_current_time();
	philo->meal_count++;
	pthread_mutex_unlock(&info->data_mutex);
	ft_sleep(philo, info->eating_duration);
	pthread_mutex_unlock(&info->forks[philo->left_fork_id]);
	pthread_mutex_unlock(&info->forks[philo->right_fork_id]);
}

void	*philo_routine(void *arg)
{
	t_philo_info	*philo;
	t_sim_info		*info;

	philo = (t_philo_info *)arg;
	info = philo->sim_info;
	if (philo->philosopher_id % 2 == 0)
		usleep(1000);
	while (has_simulation_ended(info) == NO)
	{
		philo_eat(philo);
		if (has_simulation_ended(info) == YES)
			break ;
		print_status(philo, "is sleeping", NO);
		ft_sleep(philo, info->sleeping_duration);
		print_status(philo, "is thinking", NO);
	}
	return (NULL);
}