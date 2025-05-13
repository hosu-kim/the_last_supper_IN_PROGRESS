/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 21:14:29 by hoskim            #+#    #+#             */
/*   Updated: 2025/05/13 22:24:15 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h> // timeval, gettimeofday()

typedef struct s_philo
{
	int				id;
	int				count_of_meals;
	int				left_fork_id;
	int				right_fork_id;
	long long		last_eaten_time;
	struct s_info	*shared_info_ptr;
	pthread_t		thread;
}	t_philo;

// ms == milliseconds
typedef struct s_info
{
	int				num_philos;			 // argv[1]
	int				max_ms_without_meal; // argv[2]
	int				time_to_eat;		 // argv[3]
	int				time_to_sleep;		 // argv[4]
	int				num_must_eat;		 // argv[5]
	int				num_full_philo;
	int				finish;
	long long		start_time;
	t_philo			*philos;
	pthread_mutex_t	*total_forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	eat_mutex;
	pthread_mutex_t	finish_mutex;
}	t_simulation_info;

int			print_error(char *message);
int			ft_atoi(const char *nptr);
long long	get_current_milliseconds(void);
void		print_status(t_philo *philo, const char *message);
void		ft_sleep(t_philo *philo, long long ms);

int			init_simulation_info(t_simulation_info *info, int argc, char *argv[]);
int			init_philos(t_simulation_info *info);
int			init_mutext(t_simulation_info *info);
int			create_philos(t_simulation_info *info);

int			check_dead(t_philo *philo);
int			check_finish(t_philo *philo, int yes);
void		*philo_start(void *arg);

int			free_info(t_simulation_info *info);
int			free_destroy(t_simulation_info *info);
void		join_free_destory(t_simulation_info *info);
void		destory(t_simulation_info *info);

#endif