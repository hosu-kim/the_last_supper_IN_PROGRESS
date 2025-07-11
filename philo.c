/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:22:39 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/11 18:49:51 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Makes a philosopher acquire their left and right forks.
 * 
 * To preven deadlock, the order of acquiring forks is different
 * depending on whether the philosopher's ID is odd or even:
 * 
 * 1. Odd-numbered philosophers: pick up the left fork first, then the right.
 * 2. Even-numbered philosophers: pick up the right fork first, then the left.
 * 
 * @param philo The philosopher who is acquiring the forks.
 * @param sim A pointer to the simulation struct containing the mutexes.
 */
static void	acquire_forks(t_philosopher *philo, t_simulation *sim)
{
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(&sim->fork_mutexes[philo->left_fork_index]);
		print_philosopher_status(philo, "has taken a left fork", NOT_DEAD);
		pthread_mutex_lock(&sim->fork_mutexes[philo->right_fork_index]);
		print_philosopher_status(philo, "has taken a right fork", NOT_DEAD);
	}
	else
	{
		pthread_mutex_lock(&sim->fork_mutexes[philo->right_fork_index]);
		print_philosopher_status(philo, "has taken a right fork", NOT_DEAD);
		pthread_mutex_lock(&sim->fork_mutexes[philo->left_fork_index]);
		print_philosopher_status(philo, "has taken a left fork", NOT_DEAD);
	}
}

/**
 * @brief Makes a philosopher release their left and right forks.
 * 
 * After a philosopher has finished eating, this function unlocks the mutexes
 * for both the left and right forks, making them available for
 * other philosophers to use.
 * 
 * @param philo The philosopher who is releasing the forks.
 * @param sim A pointer to the simulation structure containing the mutexes.
 */
static void	release_forks(t_philosopher *philo, t_simulation *sim)
{
	pthread_mutex_unlock(&sim->fork_mutexes[philo->left_fork_index]);
	pthread_mutex_unlock(&sim->fork_mutexes[philo->right_fork_index]);
}

/**
 * @brief Makes a philosopher sleep for a specified duration
 *        while monitoring simulation state
 * 
 * This function implements a sleep mechanism that allows the philosopher
 * to sleep for the given duration while periodically checking if the simulation
 * has ended.
 * Uses short sleep intervals to ensure responsive termination.
 * 
 * @param philo Pointer to the philosopher structure
 * @param duration_ms Sleep duration in milliseconds
 * 
 * @note Function exits early if simulation finishes before sleep completes
 * @note Uses usleep(500) for fine-grained timing control
 *       and simulation monitoring
 */
static void	philosopher_sleep(t_philosopher *philo, long long sleep_duration_ms)
{
	long long	start_time;
	long long	elapsed_time;

	start_time = get_current_time_ms();
	while (!is_simulation_finished(philo->simulation))
	{
		elapsed_time = get_current_time_ms() - start_time;
		if (elapsed_time >= sleep_duration_ms)
			break ;
		usleep(500);
	}
}

/**
 * @brief Handles the philosopher's eating routine.
 * 
 * This function first checks for the edge case of a single philosopher:
 * 
 * 1. Edge case (There's only one philosopher):
 *    The philosopher will take one fork and wait until they starve,
 *    as there's no another fork.
 *    This allows the simulation to end correctly.
 * 
 * 2. Normal case:
 *    The philosopher acquires both forks, eats for the specified `time_to_eat`,
 *    and then releases the forks.
 *    It updates the `last_meal_time` and `meals_eaten` count while protecting
 *    the shared data with a `data_mutex` to prevent race conditions.
 *
 * @param philo A pointer to the philosopher who is going to eat.
 */
static void	philosopher_eat(t_philosopher *philo)
{
	t_simulation	*sim;

	sim = philo->simulation;
	if (sim->philosopher_count == 1)
	{
		pthread_mutex_lock(&sim->fork_mutexes[philo->left_fork_index]);
		print_philosopher_status(philo, "has taken a fork", NOT_DEAD);
		philosopher_sleep(philo, sim->time_to_die + 1);
		pthread_mutex_unlock(&sim->fork_mutexes[philo->left_fork_index]);
		return ;
	}
	acquire_forks(philo, sim);
	print_philosopher_status(philo, "is eating", NOT_DEAD);
	pthread_mutex_lock(&sim->data_mutex);
	philo->last_meal_time = get_current_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&sim->data_mutex);
	philosopher_sleep(philo, sim->time_to_eat);
	release_forks(philo, sim);
}

/**
 * @brief The main lifecycle function for a philosopher thread.
 * 
 * This function serves as the entry point for each philosopher thread.
 * It continuously loops through the states of eating, sleeping, and
 * thinking until the simulation is flagged to be over.
 * 
 * Key logic:
 * 1. **Staggered Start**: `Even-numbered philosophers are delayed` initially
 *    to prevent them from all competing for forks at the same time.
 * 
 * 2. **Main Loop**: `The loop continutes` as long as the simulation is active.
 * 
 * 3. **Actions**: Inside the loop, the philosopher `eats, sleeps, and thinks`.
 * 
 * 4. **Termination Check**: The simulation's `status is checked` after
 *    each major action (eating, sleeping) to ensure the thread exits promptly
 *    when the simulation ends (e.g., due to a death or all meals eaten).
 * 
 * 5. **Synchronization**: A small delay is added for thinking when there's
 *    an odd number of philosophers to help prevent livelock scenarios.
 * 
 * @param arg A void pointer to the philosopher's `t_philosopher` structure.
 * @return Returns NULL when the thread finishes execution.
 */
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
		print_philosopher_status(philo, "is sleeping", NOT_DEAD);
		philosopher_sleep(philo, sim->time_to_sleep);
		if (is_simulation_finished(sim))
			break ;
		print_philosopher_status(philo, "is thinking", NOT_DEAD);
		if (sim->philosopher_count % 2 == 1)
			usleep(100);
	}
	return (NULL);
}
