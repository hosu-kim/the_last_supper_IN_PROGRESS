/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:38:51 by hoskim            #+#    #+#             */
/*   Updated: 2025/06/28 18:13:16 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

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
	int	i;
	int	ti;
	int	sign;
	long long output;

	i = 0;
	sign = 1;
	output = 0;
	if (num_in_string[i] == '-')
		sign = -1;
	if (num_in_string[i] == '-' || num_in_string[i] == '+')
		i++;
	while (num_in_string[i])
	{
		if (num_in_string[i] < 0 || '9' < num_in_string[i])
			return (-1);
		i++;
	}
	while (num_in_string[i]
			&& '0' <= num_in_string[i] && num_in_string[i] <= '9')
	{
		output = output * 10 + (num_in_string[i] - '0');
		i++;
	}
	return (output * sign);
}

long long	get_current_milliseconds(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return ((timeval.tv_sec * 1000) + (timeval.tv_usec / 1000));
}

void	print_status(t_philo_info *philo, const char *str)
{
	long long	time;

	pthread_mutex_lock(&philo->shared_info_ptr->print_mutex);
	if (!check_finish(philo, 0))
	{
		time = get_current_milliseconds() - philo->shared_info_ptr->start_time;
		printf("%lld %d %s\n", time, philo->id, str);
	}
	pthread_mutexunlock(&philo->shared_info_ptr->print_mutex);
	if (str[0] == 'f')
		printf("All philosophers survived.\n");
}

void	ft_sleep(t_philo_info *philo, long long ms)
{
	long long	sleep_start_time;

	sleep_start_time = get_current_milliseconds();
	while (!check_finish(philo, 0)
			&& (get_current_milliseconds() - sleep_start_time) < ms)
		usleep(100);
}
