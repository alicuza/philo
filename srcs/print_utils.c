/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 20:59:56 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/27 22:02:30 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	init_print(t_thread_data *data, uint32_t *philo_print_idx,
			int64_t *cur_time_ms)
{
	t_timeval	cur_time;

	gettimeofday(&cur_time, NULL);
	*cur_time_ms = get_time_in_ms(data->simulation_start_time, &cur_time);
	*philo_print_idx = data->philo_idx + 1;
	pthread_mutex_lock(data->print_gate);
	if (!is_simulation_running(data))
	{
		pthread_mutex_unlock(data->print_gate);
		return (false);
	}
	return (true);
}

int	print_formatted(int64_t time, uint32_t philo_print_idx, char *action)
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
	uint64_t	philo_print_idx;

	if (!init_print(data, &philo_print_idx, &cur_time_ms))
		return (false);
	(void)print_formatted(cur_time_ms, philo_print_idx, FORK);
	(void)print_formatted(cur_time_ms, philo_print_idx, EAT);
	atomic_store_explicit(&data->last_meal_time_ms, cur_time_ms,
		memory_order_relaxed);
	pthread_mutex_unlock(data->print_gate);
	return (true);
}

void	ft_putstr_fd(char *c, int fd)
{
	write(fd, c, ft_strlen(c));
}
