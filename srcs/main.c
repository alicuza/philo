/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 14:01:47 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/27 21:54:22 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	start_simulation(t_ctx *c)
{
	uint32_t	i;

	gettimeofday(&c->sim_start_time, NULL);
	i = -1;
	while (++i < c->args[NBR_PHILOS])
		pthread_mutex_unlock(c->philo_data[i].gate);
}

static bool	is_philo_dead(t_ctx *c, uint32_t idx, int64_t cur_time_ms)
{
	int64_t		last_eaten_ms;
	uint32_t	philo_print_idx;

	last_eaten_ms = atomic_load_explicit(&c->philo_data[idx].last_meal_time_ms,
			memory_order_relaxed);
	if (cur_time_ms - last_eaten_ms >= (uint64_t)c->args[T_DIE])
	{
		init_print(&c->philo_data[idx], &philo_print_idx, &cur_time_ms);
		print_formatted(cur_time_ms, philo_print_idx, DEAD);
		atomic_store(&c->ph_to_go, 0, memory_order_relaxed);
		pthread_mutex_unlock(&c->print_gate);
		return (true);
	}
	return (false);
}

static void	monitor_philos(t_ctx *c)
{
	uint32_t	i;
	t_timeval	cur_time;
	uint64_t	cur_time_ms;

	while (atomic_load_explicit(&c->ph_to_go, memory_order_relaxed) > 0)
	{
		i = -1;
		while (++i < c->args[NBR_PHILOS])
		{
			gettimeofday(&cur_time, NULL);
			cur_time_ms = get_time_in_ms(&c->sim_start_time, &cur_time);
			if (is_philo_dead(c, i, cur_time_ms))
				return ;
		}
		usleep(1000);
	}
}

static void	join_philos(t_ctx *c)
{
	uint32_t	i;

	i = -1;
	while (++i < c->args[NBR_PHILOS])
		pthread_join(c->philo_data[i].tid, NULL);
}

int	main(int argc, char **argv)
{
	t_ctx	c;

	init_context(&c, argc, argv);
	start_simulation(&c);
	monitor_philos(&c);
	join_philos(&c);
	philo_exit(&c, NULL, NULL, EXIT_SUCCESS);
}
