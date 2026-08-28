/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 17:37:55 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/28 17:39:18 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	assign_data_to_philo(t_ctx *c, uint32_t i)
{
	atomic_init(&c->philo_data[i].last_meal_time_ms, 0);
	if (!(i % 2))
	{
		c->philo_data[i].fork_first = &c->fork[i];
		c->philo_data[i].fork_second = &c->fork[(i + 1) % c->args[NBR_PHILOS]];
	}
	else
	{
		c->philo_data[i].fork_first = &c->fork[(i + 1) % c->args[NBR_PHILOS]];
		c->philo_data[i].fork_second = &c->fork[i];
	}
	c->philo_data[i].start_flag = &c->start_flag;
	c->philo_data[i].philo_idx = i;
	c->philo_data[i].sim_start_time = &c->sim_start_time;
	c->philo_data[i].print_gate = &c->print_gate;
	c->philo_data[i].ph_to_go = &c->ph_to_go;
	c->philo_data[i].args = c->args;
}

int	start_philos(t_ctx *c)
{
	int			err;
	t_routine	*r;

	if (c->args[NBR_PHILOS] == 1)
		r = routine_single_philo;
	else
		r = routine_multiple_philos;
	c->started = -1;
	while (++c->started < c->args[NBR_PHILOS])
	{
		assign_data_to_philo(c, c->started);
		err = pthread_create(&c->philo_data[c->started].tid, NULL, r,
				&c->philo_data[c->started]);
		if (err)
			return (err);
	}
	return (0);
}
