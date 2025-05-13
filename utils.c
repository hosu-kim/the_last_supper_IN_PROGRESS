/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 13:19:13 by hoskim            #+#    #+#             */
/*   Updated: 2025/05/13 22:23:12 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Prints the provided error message on the console.
 */
int	print_error(char *err_msg)
{
	while (*err_msg)
	{
		write(2, err_msg, 1);
		err_msg++;
	}
	return (1);
}

/**
 * @brief Converts a number from a string to a integer.
 */
int	ft_atoi(const char *num_in_str)
{
	int			i;
	int			ti;
	int			sign;
	long long	output;

	i = 0;
	sign = 1;
	output = 0;
	if (num_in_str[i] == '-')
		sign = -1;
	if (num_in_str[i] == '-' || num_in_str[i] == '+')
		i++;
	while (num_in_str[i])
	{
		if (num_in_str[i] < '0' || '9' < num_in_str[i])
			return (-1);
		i++;
	}
	while (num_in_str[i] && '0' <= num_in_str[i] && num_in_str[i] <= '9')
	{
		output = output * 10 + (num_in_str[i] - '0');
		i++;
	}
	return (output * sign);
}

/**
 * @details
 * --- gettimeofday() ---
 * ex. tv_sec = 1747061460, tv_usec = 714185
 * tv_sec: Represents the total seconds
 * 		   from the Unix epoch time(1970/1/1 00:00:00 UTC).
 * tv_usec: Represents the addtional microseconds
 * 			that has elapsed since the last full second
 * 
 * @return milliseconds from the Unix epoch.
 */
long long	get_current_milliseconds(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return ((timeval.tv_sec * 1000) + (timeval.tv_usec / 1000));
}

void	print_status(t_philo *philo, const char *str)
{
	long long	time;

	pthread_mutex_lock(&philo->shared_info_ptr->print_mutex);
	if (!check_finish(philo, 0))
	{
		time = get_current_milliseconds() - philo->shared_info_ptr->start_time;
		printf("%lld %d %s\n", time, philo->id, str);
	}
	pthread_mutex_unlock(&philo->shared_info_ptr->print_mutex);
	if (str[0] == 'f')
		printf("Philosophers survived!\n ");
}

void	ft_sleep(t_philo *philo, long long ms)
{
	long long	sleep_start_time;
	
	sleep_start_time = get_current_milliseconds();
	while (!check_finish(philo, 0)
			&& (get_current_milliseconds() - sleep_start_time) < ms)
		usleep(100);
}
