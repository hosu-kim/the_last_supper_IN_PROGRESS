/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:30:32 by hoskim            #+#    #+#             */
/*   Updated: 2025/06/29 15:19:11 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_philosopher_alive(t_philo_info *philosopher)
{
	pthread_mutext_lock(&philosopher->shared_resources->eat_mutex);
	if ((get_time() - philosopher->last_eaten_time)
		>= philosopher->shared_resources->max_ms_without_meal)
	{
		print_status(philosopher, "died :(");
		check_finish(philosopher, 1);
		pthread_mutex_unlock(&philosopher->shared_resources->eat_mutex);
		return (FAILURE);
	}
	else if (philosopher->shared_resources->num_must_eat > 0
		&& philosopher->count_of_meals
		>= philosopher->shared_resources->num_must_eat)
	{
		philosopher->shared_resources->num_full_philos++;
		if (philosopher->shared_resources->num_full_philos
			>= philosopher->shared_resources->num_of_philosophers)
		{
			check_finish(philosopher, 1);
			print_status(philosopher, "f");
			pthread_mutex_unlock(&philosopher->shared_resources->eat_mutex);
			return (FAILURE);
		}
	}
	pthread_mutex_unlock(&philosopher->shared_resources->eat_mutex);
	return (SUCCESS);
}

static void	eat(t_philo_info *philos)
{
	pthread_mutex_lock(&philos->shared_resources->forks[philos->left_fork]);
	print_status(philo_start, "has taken a fork");
	pthread_mutex_lock(&philos->shared_resources->forks[philos->right_fork]);
	print_status(philos, "has taken a fork");
	print_status(philo_start, "is eating");
	ft_sleep(philo_start, philos->shared_resources->duration_of_eating_ms);
	pthread_mutex_lock(&philos->shared_resources->eat_mutex);
	philos->count_of_meals += 1;
	philos->last_eaten_time = get_time();
	pthread_mutex_unlock(&philos->shared_resources->eat_mutex);
	pthread_mutex_unlock(&philos->shared_resources->forks[philos->right_fork]);
	pthread_mutex_unlock(&philos->shared_resources->forks[philos->left_fork]);
}

int	is_sim_finished(t_philo_info *philosopher, int inform_finished)
{
	pthread_mutex_lock(&philosopher->shared_resources->finish_mutex);
	if (inform_finished == YES)
	{
		philosopher->shared_resources->finish = 1;
		pthread_mutex_unlock(&philosopher->shared_resources->finish_mutex);
		return (YES);
	}
	if (philosopher->shared_resources->finish)
	{
		pthread_mutex_unlock(&philosopher->shared_resources->finish_mutex);
		return (YES);
	}
	pthread_mutex_unlock(&philosopher->shared_resources->finish_mutex);
	return (NO);
}

void	*philo_start(void *arg)
{
	t_philo_info	*philos;

	philos = (t_philo_info *)arg;
	if (philos->id % 2 == 0)
		usleep(philos->shared_resources->duration_of_eating_ms * 1000);
	while (42)
	{
		if (check_finish(philos, 0))
			return (SUCCESS);
		eat(philos);
		print_status(philos, "is sleeping");
		ft_sleep(philos, philos->shared_resources->duration_of_sleep_ms);
		print_status(philos, "is thinking");
	}
	return (SUCCESS);
}
