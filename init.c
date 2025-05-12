/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoskim <hoskim@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 22:00:07 by hoskim            #+#    #+#             */
/*   Updated: 2025/05/12 22:08:24 by hoskim           ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_info(t_info *info, int ac, char **av)
{
	int	i;
	
	i = 0;
	if (ac < 5 || ac > 6)
		return (print_error("Error: argc is not 5 or 6\n"));
	info->num_philos = ft_atoi(av[1]);
	info->max_ms_without_meal = ft_atoi(av[2]);
}