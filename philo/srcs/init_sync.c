/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sync.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 17:40:28 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/28 17:40:30 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_mutexes(t_ctx *c)
{
	int	err;

	c->forks_ready = -1;
	while (++c->forks_ready < c->args[NBR_PHILOS])
	{
		err = pthread_mutex_init(&c->fork[c->forks_ready], NULL);
		if (err)
			return (err);
	}
	err = pthread_mutex_init(&c->print_gate, NULL);
	if (err)
		return (err);
	c->print_gate_ready = true;
	return (0);
}

void	init_start_flag(t_ctx *c)
{
	atomic_init(&c->start_flag, false);
}

void	wait_for_start(t_thread_data *data)
{
	while (!atomic_load_explicit(data->start_flag, memory_order_acquire))
		usleep(50);
}

bool	is_simulation_running(t_thread_data *data)
{
	bool	res;

	res = atomic_load_explicit(data->ph_to_go, memory_order_relaxed) > 0;
	return (res);
}
