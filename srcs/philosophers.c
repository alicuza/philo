/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 14:01:47 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/23 17:44:40 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_ctx		c;
	uint32_t	i;

	init_context(&c, argc, argv);
	start_simulation(&c);
	i = -1;
	while (++i < c.args[NBR_PHILOS])
		pthread_join(c.philo_data[i].tid, NULL);
	return (0);
}
