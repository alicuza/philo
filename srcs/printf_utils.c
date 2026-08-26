/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 20:59:56 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/25 21:24:04 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	printf_mutex(t_thread_data *data, char *action)
{
	int64_t	time_in_ms;
	int		res;

	time_in_ms = get_time_in_ms(data->simulation_start_time,
			&data->last_action_time);
	if (!running_simulation(data))
		return (0);
	pthread_mutex_lock(data->print_gate);
	res = printf("%ld %u %s\n", time_in_ms, data->philo_idx, action);
	pthread_mutex_unlock(data->print_gate);
	return (res);
}

// TODO: check how to reference the same variable multiple times in printf.
int	printf_twice_mutex(t_thread_data *data, char *action, char *action2)
{
	int64_t	time_in_ms;
	int		res;

	time_in_ms = get_time_in_ms(data->simulation_start_time,
			&data->last_action_time);
	if (!running_simulation(data))
		return (0);
	pthread_mutex_lock(data->print_gate);
	res = printf("%ld %u %s\n%ld %u %s\n", time_in_ms, data->philo_idx, action,
			time_in_ms, data->philo_idx, action2);
	pthread_mutex_unlock(data->print_gate);
	return (res);
}
