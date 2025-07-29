/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:45:35 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/29 17:51:44 by hoskim           ###   ########seoul.kr  */
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
	// vvv Timestamp of the last meal start in milliseconds.
	long long		last_meal_time;
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
	// vvv Number of philosophers in the simulation; argv[1]
	int				philosopher_count;
	// vvv Time (ms) after which a philosopher dies if not eating; argv[2]
	int				time_to_die;
	// vvv Time (ms) it takes for a philosopher to eat; argv[3]
	int				time_to_eat;
	// vvv Time (ms) a philosopher sleeps after eating; argv[4]
	int				time_to_sleep;
	// vvv Number od meals each philosopher must eat (-1 if unlimited); argv[5]
	int				required_meals;
	// vvv Flag indicating if the simulation has ended.
	int				simulation_ended;
	// vvv Timestamp when the simulation started (in milliseconds).
	long long		sim_start_time;
	t_philosopher	*philosophers; // < Array of philosopher structures.
	pthread_mutex_t	*mutex_for_fork; // < Array of mutex locks for each fork.
	// vvv Mutex for synchronized printing to stdout.
	pthread_mutex_t	mutex_for_printing;
	// vvv Mutex for protecting shared data access.
	pthread_mutex_t	mutex_for_shared_data;
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

// free.c
void		monitor_simulation_and_cleanup(t_simulation *sim);

#endif
