/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:45:35 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/05 19:48:25 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

# define SUCCESS 0
# define FAILURE 1
# define TRUE 1
# define FALSE 0

typedef struct s_simulation	t_simulation;

typedef struct s_philosopher
{
	int				id;
	int				meals_eaten;
	int				left_fork_index;
	int				right_fork_index;
	long long		last_meal_time;
	t_simulation	*simulation;
	pthread_t		thread;
}	t_philosopher;

typedef struct s_simulation
{
	int				philosopher_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				required_meals;
	int				simulation_ended;
	long long		start_time;
	t_philosopher	*philosophers;
	pthread_mutex_t	*fork_mutexes;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	data_mutex;
}	t_simulation;

int			print_error(char *error_message);
int			ft_atoi(const char *str);
long long	get_current_time_ms(void);
int			is_simulation_finished(t_simulation *sim);
void		print_philosopher_status(t_philosopher *philo, const char *message, int is_death);
void		philo_sleep(t_philosopher *philo, long long duration_ms);

int			initialize_simulation(t_simulation *sim, int argc, char *argv[]);

void		*philosopher_lifecycle(void *arg);

void		monitor_simulation_and_cleanup(t_simulation *sim);

#endif
