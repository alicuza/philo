/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 12:37:20 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/27 23:02:13 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
 *	i need a smarter way of doing these number validations.
 *	idea: have a number struct that stores the number in a uint64_t,
 *	the sign, the number of significant digits.
 *	this way my ato* functions are just wrappers around a function
 *	filling this struct and another functions constructing number in
 *	a primitive type.
 */

static bool	validate_and_init_args(t_ctx *c, int argc, char **argv)
{
	int		i;
	int		arg_cnt;
	size_t	nbr_len;
	int64_t	tmp;

	arg_cnt = argc - ARG_START_IDX;
	if (arg_cnt < MIN_ARG_CNT || arg_cnt > MAX_ARG_CNT)
		return (false);
	i = -1;
	while (++i < arg_cnt)
	{
		nbr_len = sig_digits_strlen(argv[i + ARG_START_IDX]);
		if (nbr_len < 1 || nbr_len > 11)
			return (false);
		tmp = (int64_t)ft_atol(argv[i + ARG_START_IDX]);
		if (tmp <= 0 || tmp >= INT_MAX)
			return (false);
		c->args[i] = (uint32_t)tmp;
	}
	if (arg_cnt != MAX_ARG_CNT)
		c->args[NBR_MEALS] = 0;
	atomic_init(&c->ph_to_go, (int)c->args[NBR_PHILOS]);
	return (true);
}

static bool	init_mutexes(t_ctx *c)
{
	uint32_t	i;

	errno = 0;
	c->gate = (pthread_mutex_t *)malloc(c->args[NBR_PHILOS]
			* sizeof(pthread_mutex_t));
	if (!c->gate)
		return (false);
	c->fork = (pthread_mutex_t *)malloc(c->args[NBR_PHILOS]
			* sizeof(pthread_mutex_t));
	if (!c->fork)
		return (false);
	i = -1;
	while (++i < c->args[NBR_PHILOS])
	{
		pthread_mutex_init(&c->gate[i], NULL);
		pthread_mutex_init(&c->fork[i], NULL);
	}
	pthread_mutex_init(&c->print_gate, NULL);
	return (true);
}

static void	assign_data_to_philo(t_ctx *c, uint32_t i)
{
	atomic_init(&c->philo_data[i].last_meal_time_ms, 0);
	if (i % 2)
	{
		c->philo_data[i].fork_first = &c->fork[i];
		c->philo_data[i].fork_second = &c->fork[(i + 1) % c->args[NBR_PHILOS]];
	}
	else
	{
		c->philo_data[i].fork_first = &c->fork[(i + 1) % c->args[NBR_PHILOS]];
		c->philo_data[i].fork_second = &c->fork[i];
	}
	c->philo_data[i].gate = &c->gate[i];
	c->philo_data[i].philo_idx = i;
	c->philo_data[i].sim_start_time = &c->sim_start_time;
	c->philo_data[i].print_gate = &c->print_gate;
	c->philo_data[i].ph_to_go = &c->ph_to_go;
	c->philo_data[i].args = c->args;
}

static bool	init_and_start_philos(t_ctx *c)
{
	uint32_t	i;
	t_routine	*r;

	errno = 0;
	c->philo_data = (t_thread_data *)malloc(c->args[NBR_PHILOS]
			* sizeof(t_thread_data));
	if (!c->philo_data)
		return (false);
	memset(c->philo_data, 0, c->args[NBR_PHILOS] * sizeof(t_thread_data));
	if (c->args[NBR_PHILOS] == 1)
		r = routine_single_philo;
	else
		r = routine_multiple_philos;
	i = -1;
	while (++i < c->args[NBR_PHILOS])
	{
		assign_data_to_philo(c, i);
		pthread_mutex_lock(c->philo_data[i].gate);
		pthread_create(&c->philo_data[i].tid, NULL, r, &c->philo_data[i]);
	}
	return (true);
}

void	init_context(t_ctx *c, int argc, char **argv)
{
	if (!validate_and_init_args(c, argc, argv))
		philo_exit(c, "validate_and_init_args",
			"Expected 4-5 positive integers up to UINT_MAX", EXIT_FAILURE);
	if (!init_mutexes(c))
		philo_exit(c, "init_mutexes",
			strerror(errno), EXIT_FAILURE);
	if (!init_and_start_philos(c))
		philo_exit(c, "init_and_start_philos",
			strerror(errno), EXIT_FAILURE);
}
