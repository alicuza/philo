/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 17:05:15 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/28 11:26:04 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// INFO: this philo dies deterministically on time_to_die,
//       threre is no reason to poll any death flag in this case;
//       i keep the mutexes because that seems to be a general requirement;
void	*routine_single_philo(void *arg)
{
	t_thread_data	*data;

	data = (t_thread_data *)arg;
	wait_for_start(data);
	if (!guard_lock(data->ph_to_go, data->fork_first))
		return (NULL);
	if (!print_action(data, FORK))
	{
		pthread_mutex_unlock(data->fork_first);
		return (NULL);
	}
	sleep_until(data->sim_start_time, data->args[T_DIE]);
	pthread_mutex_unlock(data->fork_first);
	return (NULL);
}

static bool	philo_take_forks_and_eat(t_thread_data *data)
{
	if (!take_both_forks(data))
		return (false);
	if (!print_eat(data))
	{
		drop_forks(data);
		return (false);
	}
	sleep_until(data->sim_start_time, data->last_print_time_ms + data->args[T_EAT]);
	drop_forks(data);
	return (true);
}

static bool	philo_sleep_and_think(t_thread_data *data)
{
	int64_t	eat_end_ms;

	eat_end_ms = data->last_print_time_ms + data->args[T_EAT];
	if (!print_action(data, SLEEP))
		return (false);
	sleep_until(data->sim_start_time, eat_end_ms + data->args[T_SLEEP]);
	if (!print_action(data, THINK))
		return (false);
	sleep_until(data->sim_start_time,
		eat_end_ms + data->args[T_SLEEP] + data->time_to_think);
	return (true);
}

static uint32_t	get_time_to_think(uint32_t *args)
{
	int64_t	slack;
	int64_t	think;

	slack = (int64_t)args[T_DIE] - args[T_EAT] - args[T_SLEEP];
	if (slack <= 0)
		return (0);
	think = (int64_t)args[T_EAT] * 2 - args[T_SLEEP];
	if (think < 0)
		return (0);
	if (think > slack / 2)
		think = slack / 2;
	return ((uint32_t)think);
}

void	*routine_multiple_philos(void *arg)
{
	t_thread_data	*data;

	data = (t_thread_data *)arg;
	data->time_to_think = get_time_to_think(data->args);
	wait_for_start(data);
	if (data->philo_idx % 2)
		sleep_until(data->sim_start_time, data->args[T_EAT] / 2);
	while (true)
	{
		if (!philo_take_forks_and_eat(data))
			break ;
		if (data->args[NBR_MEALS] > 0
			&& ++data->meal_cnt == data->args[NBR_MEALS])
			atomic_fetch_sub_explicit(data->ph_to_go, 1, memory_order_relaxed);
		if (!philo_sleep_and_think(data))
			break ;
	}
	return (NULL);
}
