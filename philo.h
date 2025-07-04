/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:45:35 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/04 19:51:46 by hoskim           ###   ########seoul.kr  */
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
# define YES 1
# define NO 0

typedef struct s_sim_info	t_sim_info;

typedef struct s_philosopher_info
{
	int				philosopher_id;
	int				meal_count;
	int				last_meal_time;
	int				left_fork_id;
	int				right_fork_id;
	long long		last_eaten_time;
	t_sim_info		*sim_info;
	pthread_t		thread;
}	t_philo_info;

typedef struct s_sim_info
{
	int				num_of_philos;
	int				max_time_without_meal;
	int				eating_duration;
	int				sleeping_duration;
	int				num_must_eat;
	int				num_of_full_philos;
	int				sim_finished;
	long long		sim_start_time;
	t_philo_info	*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	data_mutex;
}	t_sim_info;

int			print_error(char *error_message);
int			ft_atoi(const char *num_in_string);
long long	get_current_time(void);
int	has_simulation_ended(t_sim_info *info);
void	print_status(t_philo_info *philo, const char *message, int is_dead);
void		ft_sleep(t_philo_info *philo, long long ms);

int	init_simulation(t_sim_info *info, int argc, char *argv[]);

void	*philo_routine(void *arg);

void	monitor_and_cleanup(t_sim_info *info);

#endif
