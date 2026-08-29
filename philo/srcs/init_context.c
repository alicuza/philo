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

int	init_context(t_ctx *c, int argc, char **argv)
{
	memset(c, 0, sizeof(t_ctx));
	if (!validate_and_init_args(c, argc, argv))
		return (philo_exit(c, "validate_args", E_ARGS, EXIT_FAILURE));
	if (alloc_simulation(c))
		return (philo_exit(c, "alloc_simulation", E_ALLOC, EXIT_FAILURE));
	if (init_mutexes(c))
		return (philo_exit(c, "init_mutexes", E_MUTEX, EXIT_FAILURE));
	init_start_flag(c);
	if (start_philos(c))
		return (philo_exit(c, "start_philos", E_THREAD, EXIT_FAILURE));
	return (EXIT_SUCCESS);
}
