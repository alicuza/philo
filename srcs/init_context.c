/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_context.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 12:37:20 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/28 17:38:54 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
		if (tmp <= 0 || tmp > UINT_MAX)
			return (false);
		c->args[i] = (uint32_t)tmp;
	}
	if (arg_cnt != MAX_ARG_CNT)
		c->args[NBR_MEALS] = 0;
	atomic_init(&c->ph_to_go, (int64_t)c->args[NBR_PHILOS]);
	return (true);
}

void	init_context(t_ctx *c, int argc, char **argv)
{
	int	err;

	memset(c, 0, sizeof(t_ctx));
	if (!validate_and_init_args(c, argc, argv))
		philo_exit(c, "validate_args",
			"Expected 4-5 positive integers up to UINT_MAX", EXIT_FAILURE);
	err = alloc_simulation(c);
	if (err)
		philo_exit(c, "alloc_simulation", strerror(err), EXIT_FAILURE);
	err = init_mutexes(c);
	if (err)
		philo_exit(c, "init_mutexes", strerror(err), EXIT_FAILURE);
	init_start_flag(c);
	err = start_philos(c);
	if (err)
		philo_exit(c, "start_philos", strerror(err), EXIT_FAILURE);
}
