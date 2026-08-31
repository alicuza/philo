/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 20:59:56 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/01 01:34:49 by sancuta          ###   ########.fr       */
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
	int64_t		cur_time_ms;
	int64_t		last_meal_ms;
	uint32_t	philo_print_idx;

	if (!init_print(data, &philo_print_idx, &cur_time_ms))
		return (false);
	last_meal_ms = atomic_load_explicit(&data->last_meal_time_ms,
			memory_order_relaxed);
	if (cur_time_ms - last_meal_ms >= (int64_t)data->args[T_DIE])
	{
		print_formatted(cur_time_ms, philo_print_idx, DEAD);
		atomic_store_explicit(data->ph_to_go, 0, memory_order_relaxed);
		pthread_mutex_unlock(data->print_gate);
		return (false);
	}
	atomic_store_explicit(&data->last_meal_time_ms, cur_time_ms,
		memory_order_relaxed);
	print_formatted(cur_time_ms, philo_print_idx, FORK);
	print_formatted(cur_time_ms, philo_print_idx, EAT);
	pthread_mutex_unlock(data->print_gate);
	return (true);
}
