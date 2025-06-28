/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 22:30:32 by hoskim            #+#    #+#             */
/*   Updated: 2025/06/28 22:52:20 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philo_info *philosopher)
{
	pthread_mutext_lock(&philosopher->shared_info_ptr->eat_mutex);
	if ((get_time() - philosopher->last_eaten_time)
		>= philosopher->shared_info_ptr->max_ms_without_meal)
	{
		print_status(philosopher, "died :(");
		check_finish(philosopher, 1);
		pthread_mutex_unlock(&philosopher->shared_info_ptr->eat_mutex);
		return (FAILURE);
	}
	else if (philosopher->shared_info_ptr->num_must_eat > 0
			&& philosopher->count_of_meals
			>= philosopher->shared_info_ptr->num_must_eat)
	{
		philosopher->shared_info_ptr->num_full_philos++;
		if (philosopher->shared_info_ptr->num_full_philos
			>= philosopher->shared_info_ptr->num_of_philosophers)
		{
			check_finish(philosopher, 1);
			print_status(philosopher, "f");
			pthread_mutex_unlock(&philosopher->shared_info_ptr->eat_mutex);
			return (FAILURE);
		}
	}
	pthread_mutex_unlock(&philosopher->shared_info_ptr->eat_mutex);
	return (SUCCESS);
}

static void	eat(t_philo_info *philosopher)
{
	pthread_mutex_lock(&philosopher->shared_info_ptr->forks[philosopher->left_fork]);
	print_status(philo_start, "has taken a fork");
	pthread_mutex_lock(&philosopher->shared_info_ptr->forks[philosopher->right_fork]);
	print_status(philosopher, "has taken a fork");
	print_status(philo_start, "is eating");
	ft_sleep(philo_start, philosopher->shared_info_ptr->duration_of_eating_ms);
	pthread_mutex_lock(&philosopher->shared_info_ptr->eat_mutex);
	philosopher->count_of_meals += 1;
	philosopher->last_eaten_time = get_time();
	pthread_mutex_unlock(&philosopher->shared_info_ptr->eat_mutex);
	pthread_mutex_unlock(&philosopher->shared_info_ptr->forks[philosopher->right_fork]);
	pthread_mutex_unlock(&philosopher->shared_info_ptr->forks[philosopher->left_fork]);
}

int	check_finish(t_philo_info *philosopher, int yes)
{
	pthread_mutex_lock(&philosopher->shared_info_ptr->finish_mutex);
	if (yes)
	{
		philosopher->shared_info_ptr->finish = 1;
		pthread_mutex_unlock(&philosopher->shared_info_ptr->finish_mutex);
		return (FAILURE);
	}
	if (philosopher->shared_info_ptr->finish)
	{
		pthread_mutex_unlock(&philosopher->shared_info_ptr->finish_mutex);
		return (FAILURE);
	}
	pthread_mutex_unlock(&philosopher->shared_info_ptr->finish_mutex);
	return (SUCCESS);
}

void	*philo_start(void *arg)
{
	t_philo_info	*philosopher;

	philosopher = (t_philo_info *)arg;
	if (philosopher->id % 2 == 0)
		usleep(philosopher->shared_info_ptr->duration_of_eating_ms * 1000);
	while (42)
	{
		if (check_finish(philosopher, 0))
			return (SUCCESS);
		eat(philosopher);
		print_status(philosopher, "is sleeping");
		ft_sleep(philosopher, philosopher->shared_info_ptr->duration_of_sleep_ms);
		print_status(philosopher, "is thinking");
	}
	return (SUCCESS);
	
}