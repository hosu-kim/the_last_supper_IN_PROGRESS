/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:38:51 by hoskim            #+#    #+#             */
/*   Updated: 2025/06/29 18:41:39 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

// 2025-06-29 15:34:00: code review completed
#include "philo.h"

int	print_error(char *error_message)
{
	while (*error_message)
	{
		write(2, error_message, 1);
		error_message++;
	}
	return (FAILURE);
}

int	ft_atoi(const char *num_in_string)
{
	int			i;
	int			sign;
	long long	output;

	i = 0;
	sign = 1;
	output = 0;
	if (num_in_string[i] == '-')
		sign = -1;
	if (num_in_string[i] == '-' || num_in_string[i] == '+')
		i++;
	while (num_in_string[i])
	{
		if (num_in_string[i] < '0' || '9' < num_in_string[i])
			return (FAILURE);
		if ('0' <= num_in_string[i] && num_in_string[i] <= '9')
			output = output * 10 + (num_in_string[i] - '0');
		i++;
	}
	return (output * sign);
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

void	print_status(t_philo_info *philo, const char *message)
{
	long long	elapsed_time;

	pthread_mutex_lock(&philo->shared_resources->print_mutex);
	if (!is_sim_finished(philo, 0))
	{
		elapsed_time \
			= get_current_time() - philo->shared_resources->sim_start_time;
		printf("%lld %d %s\n", elapsed_time, philo->philosopher_id, message);
	}
	pthread_mutexunlock(&philo->shared_resources->print_mutex);
	if (message[0] == 'f')
		printf("All philosophers survived.\n");
}

void	ft_sleep(t_philo_info *philo, long long sleep_duration)
{
	long long	start_time_to_sleep;

	start_time_to_sleep = get_current_time();
	while (is_sim_finished(philo, NO) == NO
		&& (get_current_time() - start_time_to_sleep) < sleep_duration)
		usleep(100);
}
