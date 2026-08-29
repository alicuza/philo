/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 20:59:56 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/28 17:36:27 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	init_print(t_thread_data *data, uint32_t *philo_print_idx,
			int64_t *cur_time_ms)
{
	t_timeval	cur_time;

	*philo_print_idx = data->philo_idx + 1;
	if (!guard_lock(data->ph_to_go, data->print_gate))
		return (false);
	gettimeofday(&cur_time, NULL);
	*cur_time_ms = get_time_in_ms(data->sim_start_time, &cur_time);
	if (!is_simulation_running(data))
	{
		pthread_mutex_unlock(data->print_gate);
		return (false);
	}
	return (true);
}

inline int	print_formatted(int64_t time, uint32_t philo_print_idx,
			char *action)
{
	int	res;

	res = printf("%ld %u %s\n", time, philo_print_idx, action);
	return (res);
}

bool	print_action(t_thread_data *data, char *action)
{
	int64_t		cur_time_ms;
	uint32_t	philo_print_idx;

	if (!init_print(data, &philo_print_idx, &cur_time_ms))
		return (false);
	(void)print_formatted(cur_time_ms, philo_print_idx, action);
	pthread_mutex_unlock(data->print_gate);
	return (true);
}

bool	print_eat(t_thread_data *data)
{
	int64_t		meal_time_ms;
	int64_t		print_time_ms;
	uint32_t	philo_print_idx;
	t_timeval	cur_time;

	gettimeofday(&cur_time, NULL);
	meal_time_ms = get_time_in_ms(data->sim_start_time, &cur_time);
	atomic_store_explicit(&data->last_meal_time_ms, meal_time_ms,
		memory_order_relaxed);
	if (!init_print(data, &philo_print_idx, &print_time_ms))
		return (false);
	(void)print_formatted(print_time_ms, philo_print_idx, FORK);
	(void)print_formatted(print_time_ms, philo_print_idx, EAT);
	pthread_mutex_unlock(data->print_gate);
	return (true);
}
