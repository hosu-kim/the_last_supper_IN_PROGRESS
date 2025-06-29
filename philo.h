/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 16:45:35 by hoskim            #+#    #+#             */
/*   Updated: 2025/06/29 18:53:10 by hoskim           ###   ########seoul.kr  */
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
# define FAILURE -1
# define YES 1
# define NO 0

typedef struct s_philosophter_info
{
	int					philosopher_id;
	int					last_meal_time;
	int					left_fork_id;
	int					right_fork_id;
	long long			last_eaten_time;
	t_simulation_info	*shared_resources;
	pthread_t			thread;
}	t_philo_info;

typedef struct s_simulation_info
{
	int				num_of_philos;
	int				max_time_without_meal;
	int				eating_duration;
	int				sleeping_duration;
	int				num_of_philos_must_eat;
	int				num_of_philos;
	int				is_sim_finished;
	long long		sim_start_time;
	t_philo_info	*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	eat_mutex;
	pthread_mutex_t	finish_mutex;
}	t_simulation_info;

int			print_error(char *error_message);
int			ft_atoi(const char *num_in_string);
long long	get_current_time(void);
void		print_status(t_philo_info *philo, const char *message);
void		ft_sleep(t_philo_info *philo, long long ms);

int			init_simulation_info(
				t_simulation_info *info,
				int argc,
				char *argv[]
				);
int			init_philo_info(t_simulation_info *info);
int			init_mutex(t_simulation_info *info);
int			create_philosophers(t_simulation_info *info);

int			is_philosopher_alive(t_philo_info *philo);
int			is_sim_finished(t_philo_info *philo, int yes);
void		*philo_start(void *arg);

int			free_info(t_simulation_info *info);
int			free_destroy(t_simulation_info *info);
void		join_free_destroy(t_simulation_info *info);
void		destroy(t_simulation_info *info);

#endif
