/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:02:21 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/27 22:14:21 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	is_simulation_running(t_thread_data *data)
{
	bool	res;

	res = atomic_load_explicit(data->ph_to_go, memory_order_relaxed) > 0;
	return (res);
}

int64_t	get_time_in_ms(t_timeval *start, t_timeval *end)
{
	int64_t	time_in_ms;

	time_in_ms = (end->tv_sec - start->tv_sec) * 1000
		+ (end->tv_usec - start->tv_usec) / 1000;
	return (time_in_ms);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
