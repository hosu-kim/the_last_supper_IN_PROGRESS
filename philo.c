/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:30:32 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/01 20:57:40 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_philosopher_alive(t_philo_info *philosopher)
{
	pthread_mutext_lock(&philosopher->shared_resources->eat_mutex);
	if ((get_time() - philosopher->last_eaten_time)
		>= philosopher->shared_resources->max_time_without_meal)
	{
		print_status(philosopher, "died :(");
		check_finish(philosopher, 1);
		pthread_mutex_unlock(&philosopher->shared_resources->eat_mutex);
		return (FAILURE);
	}
	else if (philosopher->shared_resources->num_must_eat > 0
		&& philosopher->last_meal_time
		>= philosopher->shared_resources->num_must_eat)
	{
		philosopher->shared_resources->num_of_full_philos++;
		if (philosopher->shared_resources->num_of_full_philos
			>= philosopher->shared_resources->num_of_philos)
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
	pthread_mutex_lock(&philos->shared_resources->forks[philos->left_fork_id]);
	print_status(philo_start, "has taken a fork");
	pthread_mutex_lock(&philos->shared_resources->forks[philos->right_fork_id]);
	print_status(philos, "has taken a fork");
	print_status(philo_start, "is eating");
	ft_sleep(philo_start, philos->shared_resources->eating_duration);
	pthread_mutex_lock(&philos->shared_resources->eat_mutex);
	philos->last_meal_time += 1;
	philos->last_eaten_time = get_time();
	pthread_mutex_unlock(&philos->shared_resources->eat_mutex);
	pthread_mutex_unlock(&philos->shared_resources->forks[philos->right_fork_id]);
	pthread_mutex_unlock(&philos->shared_resources->forks[philos->left_fork_id]);
}

int	is_sim_finished(t_philo_info *philosopher, int inform_finished)
{
	pthread_mutex_lock(&philosopher->shared_resources->finish_mutex);
	if (inform_finished == YES)
	{
		philosopher->shared_resources->is_sim_finished = 1;
		pthread_mutex_unlock(&philosopher->shared_resources->finish_mutex);
		return (YES);
	}
	if (philosopher->shared_resources->is_sim_finished)
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
	if (philos->philosopher_id % 2 == 0)
		usleep(philos->shared_resources->eating_duration * 1000);
	while (42)
	{
		if (check_finish(philos, 0))
			return (SUCCESS);
		eat(philos);
		print_status(philos, "is sleeping");
		ft_sleep(philos, philos->shared_resources->sleeping_duration);
		print_status(philos, "is thinking");
	}
	return (SUCCESS);
}
