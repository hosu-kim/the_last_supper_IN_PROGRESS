/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:38:51 by hoskim            #+#    #+#             */
/*   Updated: 2025/07/27 20:09:58 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief `Prints` the provided `error message` to standard error (=2) output.
 * 
 * This function calulates the length of the error message and writes it
 * to file descriptor (stderr=2), then returns FAILURE (=1).
 * 
 * @param error_message Null-terminated string containing the error message
 * @return FAILURE indicating the error condition (=1)
 */
int	print_error(char *error_message)
{
	int	len;

	len = 0;
	while (error_message[len])
		len++;
	write(2, error_message, len);
	return (FAILURE);
}

/**
 * @brief Converts a string to an integer (atoi implementation)
 * 
 * Parses the string and converts it to an integer, handling leading
 * whitespace, optional sign, and consecutive digits.
 * 
 * Uses long long (64-bit) for intermediate calculations to prevent overflow
 * during conversion, then safely casts to int for the final result.
 * 
 * @param str Null-terminated string to convert
 * @return Converted integer value, or 0 if no valid conversion possible
 */
int	ft_atoi(const char *str)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
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
 * @brief Gets the current time in milliseconds.
 * 
 * This function utilizes the gettimeofday() system call to retrieve the
 * time elapsed since the Unix epoch (1970-01-01 00:00:00 UTC).
 * 
 * The time is stored in a `timeeval` structure, which contains:
 * - tv_sec: The number of seconds.
 * - tv_usec: The number of additional microseconds.
 * 
 * The function then converts these values into a single millisecond value.
 * 
 * @return The current time in milliseconds since the Epoch.
 * @note The second parameter (timezone) of gettimeofday() is set to NULL
 *       because the parameter is not used anymore.
 */
long long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * @brief Checks if the simulation has finished.
 * 
 * This function safely checks the `simulation_ended` flag within the
 * `t_simulation` structure using a mutex to ensure thread-safe access.
 * 
 * @param sim A pointer to the simulation structure.
 * @return An integer inicating whether the simulation has ended
 *         (1 for finished, 0 for not finished.)
 */
int	is_simulation_finished(t_simulation *sim)
{
	int	finished;

	pthread_mutex_lock(&sim->data_mutex);
	finished = sim->simulation_ended;
	pthread_mutex_unlock(&sim->data_mutex);
	return (finished);
}

/**
 * @brief Prints the current `status message` of a philosopher with `timestamp`.
 * 
 * This function safely prints philosopher status messages (eating, sleeping,
 * thinking, taking fork, or dying) with proper synchronization using mutex.
 * The output format is: "[timestamp] [philosopher_id] [message]"
 * 
 * Special handling for death messages:
 * - Death messages are always printed even after simulation ends
 * - Death message automatically sets simulation_ended flag to TRUE
 * - Regular status messages are supperessed once simulation ends
 * 
 * @param philo Pointer to the philosopher structure
 * @param message Status message to print (e.g., "is eating", "died")
 * @param is_death Boolean flag indicating if the status message is
 *                 a death message (1 for death, 0 for normal status)
 * @note
 * if (sim->simulation_ended == FALSE || is_dead == TRUE)
 *  1. sim->simulation_ended == FALSE : Prints general status messages.
 *  2. is_dead == TRUE : Prints death message.
 */
void	print_timestamp_and_philo_status_msg(
	t_philosopher *philo, const char *message, int is_dead)
{
	long long		elapsed_time;
	t_simulation	*sim;

	sim = philo->simulation;
	pthread_mutex_lock(&sim->mutex_for_printing);
	if (sim->simulation_ended == FALSE || is_dead == TRUE)
	{
		elapsed_time = get_current_time_ms() - sim->sim_start_time;
		printf("%lld %d %s\n", elapsed_time, philo->id, message);
		if (is_dead == TRUE)
			sim->simulation_ended = TRUE;
	}
	pthread_mutex_unlock(&sim->mutex_for_printing);
}
