/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 12:37:20 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/23 20:40:19 by sancuta          ###   ########.fr       */
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

// TODO: if i add error messages directly on return, the function can return a bool
//       and the defines can be used for the error message string
static int	are_invalid_args(t_ctx *c, int argc, char **argv)
{
	int		i;
	int		arg_cnt;
	size_t	nbr_len;
	int64_t	tmp;

	arg_cnt = argc - ARG_START_IDX;
	if (arg_cnt < MIN_ARG_CNT || arg_cnt > MAX_ARG_CNT)
		return (INVALID_ARG_CNT);	// TODO: potentially error message here.
	i = -1;
	while (++i < arg_cnt)
	{
		nbr_len = sig_digits_strlen(argv[i + ARG_START_IDX]);
		if (nbr_len < 1 || nbr_len > 11)
			return (INVALID_NMB);	// TODO: potentially error message here.
		tmp = (int64_t)ft_atol(argv[i + ARG_START_IDX]);
		if (tmp <= 0 || tmp >= INT_MAX)
			return (INVALID_RNG);	// TODO: potentially error message here.
		c->args[i] = (uint32_t)tmp;
	}
	if (arg_cnt != MAX_ARG_CNT)
		c->args[NBR_MEALS] = 0;
	return (VALID);
}

static bool	init_mutexes(t_ctx *c)
{
	uint32_t	i;

	c->gate = (pthread_mutex_t *)malloc(c->args[NBR_PHILOS]
			* sizeof(pthread_mutex_t));
	if (!c->gate)
		return (false);	// TODO: potentially error message here.
	c->fork = (pthread_mutex_t *)malloc(c->args[NBR_PHILOS]
			* sizeof(pthread_mutex_t));
	if (!c->fork)
		return (false);	// TODO: potentially error message here.
	i = -1;
	while (++i < c->args[NBR_PHILOS])
	{
		pthread_mutex_init(&c->gate[i], NULL);
		pthread_mutex_init(&c->fork[i], NULL);
	}
	pthread_mutex_init(&c->print_gate, NULL);
	return (true);
}

static bool	init_philo_data_and_thread(t_ctx *c)
{
	uint32_t	i;

	c->philo_data = (t_thread_data *)malloc(c->args[NBR_PHILOS]
			* sizeof(t_thread_data));
	if (!c->philo_data)
		return (false);	// TODO: potentially error message here.
	memset(c->philo_data, 0, c->args[NBR_PHILOS] * sizeof(t_thread_data));
	i = -1;
	while (++i < c->args[NBR_PHILOS])
	{
		c->philo_data[i].philo_idx = i;
		c->philo_data[i].simulation_start_time = &c->simulation_start_time;
		c->philo_data[i].last_meal_time = c->simulation_start_time;
		c->philo_data[i].last_action_time = c->simulation_start_time;
		c->philo_data[i].gate = &c->gate[i];
		c->philo_data[i].print_gate = &c->print_gate;
		c->philo_data[i].fork_left = &c->fork[i];
		c->philo_data[i].fork_right = &c->fork[(i + 1) % c->args[NBR_PHILOS]];
		pthread_mutex_lock(c->philo_data[i].gate);
		pthread_create(&c->philo_data[i].tid, NULL, routine, &c->philo_data[i]);
	}
	return (true);
}

// TODO: consider putting the error messages and/or cleanup at the failure points, for more granular control
void	init_context(t_ctx *c, int argc, char **argv)
{
	if (are_invalid_args(c, argc, argv))
		exit(EXIT_FAILURE);	// TODO: add error message & cleanup
	if (!init_mutexes(c))
		exit(EXIT_FAILURE);	// TODO: add error message & cleanup
	if (!init_philo_data_and_thread(c))
		exit(EXIT_FAILURE);	// TODO: add error message & cleanup
}

void	start_simulation(t_ctx *c)
{
	uint32_t	i;

	gettimeofday(&c->simulation_start_time, NULL);
	i = -1;
	while (++i < c->args[NBR_PHILOS])
		pthread_mutex_unlock(c->philo_data[i].gate);
}
