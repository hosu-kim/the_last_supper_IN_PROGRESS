/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:38:51 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/04 19:51:30 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

// 2025-06-29 15:34:00: code review completed
#include "philo.h"

int	print_error(char *error_message)
{
	int	len;

	len = 0;
	while (error_message[len])
		len++;
	write(2, error_message, len);
	return (FAILURE);
}

int	ft_atoi(const char *num_in_string)
{
	long long	res;
	int			sign;

	res = 0;
	sign = 1;
	while (*num_in_string == ' ' || (*num_in_string >= 9 && *num_in_string <= 13))
		num_in_string++;
	if (*num_in_string == '-')
		sign = -1;
	if (*num_in_string == '-' || *num_in_string == '+')
		num_in_string++;
	while (*num_in_string >= '0' && *num_in_string <= '9')
	{
		res = res * 10 + (*num_in_string - '0');
		num_in_string++;
	}
	return ((int)(res * sign));
}

/**
 * @brief Gets the current time.
 * @note gettimeofday(struct timeval *tv, struct timezone *tz):
 *  Stores the current time since the Unix epoch (1970-01-01 00:00:00 UTC)
 *  into the timeval structure: tv_sec for seconds,
 *  and tv_usec for microseconds.
 */
long long	get_current_time(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return ((timeval.tv_sec * 1000) + (timeval.tv_usec / 1000));
}

int	has_simulation_ended(t_sim_info *info)
{
	int	result;

	pthread_mutex_lock(&info->data_mutex);
	result = info->sim_finished;
	pthread_mutex_unlock(&info->data_mutex);
	return (result);
}

void	print_status(t_philo_info *philo, const char *message, int is_dead)
{
	long long	elapsed_time;

	pthread_mutex_lock(&philo->sim_info->print_mutex);
	if (has_simulation_ended(philo->sim_info) == NO || is_dead)
	{
		elapsed_time = get_current_time() - philo->sim_info->sim_start_time;
		printf("%lld %d %s\n", elapsed_time, philo->philosopher_id, message);
	}
	pthread_mutex_unlock(&philo->sim_info->print_mutex);
}

void	ft_sleep(t_philo_info *philo, long long sleep_duration)
{
	long long	start_time;

	start_time = get_current_time();
	while (has_simulation_ended(philo->sim_info) == NO)
	{
		if ((get_current_time() - start_time) >= sleep_duration)
			break ;
		usleep(100);
	}
}
