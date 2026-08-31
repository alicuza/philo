/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 01:30:26 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/31 22:05:24 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	guard_lock(atomic_int_least64_t *ph_to_go, pthread_mutex_t *mutex)
{
	if (pthread_mutex_lock(mutex) == 0)
		return (true);
	atomic_store_explicit(ph_to_go, 0, memory_order_relaxed);
	return (false);
}

bool	take_both_forks(t_thread_data *data)
{
	if (!guard_lock(data->ph_to_go, data->fork_first))
		return (false);
	if (!print_action(data, FORK))
	{
		pthread_mutex_unlock(data->fork_first);
		return (false);
	}
	if (!guard_lock(data->ph_to_go, data->fork_second))
	{
		pthread_mutex_unlock(data->fork_first);
		return (false);
	}
	return (true);
}
