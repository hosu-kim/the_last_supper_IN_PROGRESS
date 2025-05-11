/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 21:14:29 by hoskim            #+#    #+#             */
/*   Updated: 2025/05/11 21:24:31 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				id;
	int				eat_cnt;
	int				left_fork;
	int				right_fork;
	long long		last_eaten_time;
	struct s_info	*info;
	pthread_t		thread;
}	t_philo;

typedef struct s_info
{
	int	num_philos;
	int	time_to_die;
	int	time_to_die;
	int	time_to_sleep;
	int	num_must_eat;
	int	num_full_philo;
	int	finish;
	long long	start_time;
	t_philo	*philos;
	pthread_mutext_t
}