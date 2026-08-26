/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 17:05:15 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/26 14:16:30 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

routine_order(t_thread_data *data, pthread_mutex_t first, pthread_mutex_t second)
{
		pthread_mutex_lock(first);
		gettimeofday(&data->last_action_time, NULL);
		printf_mutex(data, FORK);
		pthread_mutex_lock(second);
		gettimeofday(&data->last_action_time, NULL);
		printf_twice_mutex(data, FORK, EAT);
		usleep(1000 * data->args[T_EAT]); // TODO: is this the best approach?
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		printf_mutex(data, SLEEP);
		usleep(1000 * data->args[T_SLEEP]);
		printf_mutex(data, THINK);
}

void routine_mixed(t_thread_data *data)
{
	bool	left_first;

	left_first = true;
	while (running_simulation(data))
	{
		if (left_first)
			routine_order(data, data->fork_left, data->fork_right);
		else
			routine_order(data, data->fork_right, data->fork_left);
		left_first ~= left_first;
	}
}

/*
 * 1. where should i actually print the status?
 *    after i release the fork mutexes?
 * 2. should i have an even/uneven/mixed routine,
 *    i branch on before unlocking the gate?
 */
void	*routine(void *arg)
{
	t_thread_data	*data;

	data = (t_thread_data *)arg;
	if (is_uneven(data->philo_idx))
	{
		if (data->philo_idx == data->philo_cnt - 1)
		{
			pthread_mutex_lock(data->gate);
			pthread_mutex_unlock(data->gate);
			routine_mixed(data);
		}
		pthread_mutex_lock(data->gate);
		pthread_mutex_unlock(data->gate);
		while (running_simulation(data))
			routine_order(data, data->fork_left, data->fork_right);
	}
	else
	{
		pthread_mutex_lock(data->gate);
		pthread_mutex_unlock(data->gate);
		while (running_simulation(data))
			routine_order(data, data->fork_right, data->fork_left);
	}
	return (NULL);
}
