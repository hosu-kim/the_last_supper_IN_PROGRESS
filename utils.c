/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:38:51 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/05 18:37:20 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

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

int	ft_atoi(const char *str)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return ((int)(result * sign));
}

/**
 * @brief Gets the current time.
 * @note gettimeofday(struct timeval *tv, struct timezone *tz):
 *        Stores the current time since the Unix epoch (1970-01-01 00:00:00 UTC)
 *        into the timeval structure: tv_sec for seconds, and tv_usec for microseconds.
 */
long long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	is_simulation_finished(t_simulation *sim)
{
	int	finished;

	pthread_mutex_lock(&sim->data_mutex);
	finished = sim->simulation_ended;
	pthread_mutex_unlock(&sim->data_mutex);
	return (finished);
}

void	print_philosopher_status(t_philosopher *philo, const char *message, int is_death)
{
	long long		elapsed_time;
	t_simulation	*sim;

	sim = philo->simulation;
	pthread_mutex_lock(&sim->print_mutex);
	if (!sim->simulation_ended || is_death)
	{
		elapsed_time = get_current_time_ms() - sim->start_time;
		printf("%lld %d %s\n", elapsed_time, philo->id, message);
		if (is_death)
			sim->simulation_ended = TRUE;
	}
	pthread_mutex_unlock(&sim->print_mutex);
}

void	philo_sleep(t_philosopher *philo, long long duration_ms)
{
	long long	start_time;
	long long	elapsed_time;

	start_time = get_current_time_ms();
	while (!is_simulation_finished(philo->simulation))
	{
		elapsed_time = get_current_time_ms() - start_time;
		if (elapsed_time >= duration_ms)
			break ;
		usleep(500);
	}
}
