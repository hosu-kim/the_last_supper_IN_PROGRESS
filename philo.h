/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:45:35 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/16 00:11:53 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h> // malloc(), free()
# include <stdio.h> // printf()
# include <pthread.h> // pthread_create(), pthread_mutex_lock()...
# include <unistd.h> // write(), usleep()
# include <sys/time.h> // gettimeofday(), struct timeval

# define SUCCESS 0
# define FAILURE 1
# define TRUE 1
# define FALSE 0
# define DEAD 1
# define NOT_DEAD 0

typedef struct s_simulation	t_simulation;

/**
 * @brief Philosopher structure
 * 
 * Represents a philosopher in the Dining Philosophers Problem.
 * Each philosopher has a unique ID, meal count, fork indices, last meal time,
 * and runs on its own thread.
 */
typedef struct s_philosopher
{
	int				id;
	// ^^^ Unique identifier for the philosopher (starting from 1).
	int				meals_eaten; // < Number of meals eaten so far.
	int				left_fork_index; // < Index of the left fork.
	int				right_fork_index; // < Index of the right fork.
	long long		last_meal_time;
	// ^^^ Timestamp of the last meal start in milliseconds.
	t_simulation	*simulation; // < Pointer to the simulation data.
	pthread_t		thread; // < Thread handle for this philosopher.
}	t_philosopher;

/**
 * @brief Simulation structure for the Dining Philosophers Problem
 * 
 * Contains all the configuration parameters and shared resources needed
 * for the dining philosophers simulation. This structure is shared among
 * all philosopher threads and contains synchronization primitives.
 */
typedef struct s_simulation
{
	int				philosopher_count;
	// ^^^ Number of philosophers in the simulation; argv[1]
	int				time_to_die;
	// ^^^ Time (ms) after which a philosopher dies if not eating; argv[2]
	int				time_to_eat;
	// ^^^ Time (ms) it takes for a philosopher to eat; argv[3]
	int				time_to_sleep;
	// ^^^ Time (ms) a philosopher sleeps after eating; argv[4]
	int				required_meals;
	// ^^^ Number od meals each philosopher must eat (-1 if unlimited); argv[5]
	int				simulation_ended;
	// ^^^ Flag indicating if the simulation has ended.
	long long		sim_start_time;
	// ^^^ Timestamp when the simulation started (in milliseconds).
	t_philosopher	*philosophers; // < Array of philosopher structures.
	pthread_mutex_t	*fork_mutexes; // < Array of mutex locks for each fork.
	pthread_mutex_t	print_mutex; // < Mutex for synchronized printing to stdout.
	pthread_mutex_t	data_mutex; // < Mutex for protecting shared data access.
}	t_simulation;

// utils.c
int			print_error(char *error_message);
int			ft_atoi(const char *str);
long long	get_current_time_ms(void);
int			is_simulation_finished(t_simulation *sim);
void		print_timestamp_and_philo_status_msg(
				t_philosopher *philo, const char *message, int is_dead);

// init.c
int			initialize_simulation(t_simulation *sim, int argc, char *argv[]);

// philo.c
void		*philosopher_lifecycle(void *arg);

void		monitor_simulation_and_cleanup(t_simulation *sim);

#endif
