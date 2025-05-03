/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   models.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 17:08:52 by hoskim            #+#    #+#             */
/*   Updated: 2025/05/03 17:30:00 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODELS_H
# define MODELS_H

#include <pthread.h>

/**
 * t_fork
 * Represnets a single fork as a mutex-protected resource.
 */
typedef struct s_fork {
	pthread_mutex_t	mutex;
}	t_fork;

/**
 * t_simulation
 * Holds global configuration and shared resources
 * for the dining philosophers simulation.
 * 
 * num_philosophers: total number of philosophers (and forks)
 * time_to_die:      miliseconds before a philosopher starves if not eaten
 * time_to_eat:      miliseconds a philosopher spends eating
 * time_to_sleep:    miliseconds a philosopher spends sleeping
 * meals_to_eat:     number of meals each philosopher must eat (0 = unlimited)
 * forks:            array of t_fork, length == num_philosophers
 * print_mutex:      mutex to synchronize console output
 */
typedef struct s_simulation
{
	int				num_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				meals_to_eat;
	t_fork			*forks;
	pthread_mutex_t	print_mutex;
}	t_simulation;

/**
 * t_philosopher
 * Prepresents a single philosopher thread and its state
 * 
 * id:             unique identifier (int the range 1 to num_philosophers (inclusive).
 * left_fork_idx:  index into forks[] for left fork
 * right_fork_idx: index into forks[] for right fork
 * meals_eaten:    how many times this philosopher has eaten
 * last_meal_ts:   timestamp in milliseconds of the last meal
 * thread:         pthread handle for this philosopher
 * sim:            pointer back to the shared t_simulation
 */
typedef struct s_philosopher
{
	int				id;
	int				left_fork_idx;
	int				right_fork_idx;
	int				meals_eaten;
	long			last_meal_ts;
	pthread_t		thread;
	t_simulation	*sim;
}	t_philosopher;

#endif
