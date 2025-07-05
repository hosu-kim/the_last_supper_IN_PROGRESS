/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:22:39 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/05 19:48:25 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	acquire_forks(t_philosopher *philo, t_simulation *sim)
{
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(&sim->fork_mutexes[philo->left_fork_index]);
		print_philosopher_status(philo, "has taken a fork", FALSE);
		pthread_mutex_lock(&sim->fork_mutexes[philo->right_fork_index]);
		print_philosopher_status(philo, "has taken a fork", FALSE);
	}
	else
	{
		pthread_mutex_lock(&sim->fork_mutexes[philo->right_fork_index]);
		print_philosopher_status(philo, "has taken a fork", FALSE);
		pthread_mutex_lock(&sim->fork_mutexes[philo->left_fork_index]);
		print_philosopher_status(philo, "has taken a fork", FALSE);
	}
}

static void	release_forks(t_philosopher *philo, t_simulation *sim)
{
	pthread_mutex_unlock(&sim->fork_mutexes[philo->left_fork_index]);
	pthread_mutex_unlock(&sim->fork_mutexes[philo->right_fork_index]);
}

static void	philosopher_eat(t_philosopher *philo)
{
	t_simulation	*sim;
	
	sim = philo->simulation;
	if (sim->philosopher_count == 1)
	{
		pthread_mutex_lock(&sim->fork_mutexes[philo->left_fork_index]);
		print_philosopher_status(philo, "has taken a fork", FALSE);
		philo_sleep(philo, sim->time_to_die + 1);
		pthread_mutex_unlock(&sim->fork_mutexes[philo->left_fork_index]);
		return ;
	}
	
	acquire_forks(philo, sim);
	print_philosopher_status(philo, "is eating", FALSE);
	
	pthread_mutex_lock(&sim->data_mutex);
	philo->last_meal_time = get_current_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&sim->data_mutex);
	
	philo_sleep(philo, sim->time_to_eat);
	release_forks(philo, sim);
}

void	*philosopher_lifecycle(void *arg)
{
	t_philosopher	*philo;
	t_simulation	*sim;

	philo = (t_philosopher *)arg;
	sim = philo->simulation;
	if (philo->id % 2 == 0)
		usleep(sim->time_to_eat / 2);
	while (!is_simulation_finished(sim))
	{
		philosopher_eat(philo);
		if (is_simulation_finished(sim))
			break ;
		print_philosopher_status(philo, "is sleeping", FALSE);
		philo_sleep(philo, sim->time_to_sleep);
		if (is_simulation_finished(sim))
			break ;
		print_philosopher_status(philo, "is thinking", FALSE);
		if (sim->philosopher_count % 2 == 1)
			usleep(100);
	}
	return (NULL);
}
