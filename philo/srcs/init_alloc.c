/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_alloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 17:40:13 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/28 17:40:15 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	alloc_simulation(t_ctx *c)
{
	c->fork = (pthread_mutex_t *)malloc(c->args[NBR_PHILOS]
			* sizeof(pthread_mutex_t));
	if (!c->fork)
		return (ENOMEM);
	c->philo_data = (t_thread_data *)malloc(c->args[NBR_PHILOS]
			* sizeof(t_thread_data));
	if (!c->philo_data)
		return (ENOMEM);
	memset(c->philo_data, 0, c->args[NBR_PHILOS] * sizeof(t_thread_data));
	return (0);
}
