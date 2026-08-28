/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 19:12:33 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/28 01:47:31 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	philo_destroy_mutexes(t_ctx *c)
{
	uint32_t	i;

	i = -1;
	while (++i < c->args[NBR_PHILOS])
	{
		if (c->gate)
			pthread_mutex_destroy(&c->gate[i]);
		if (c->fork)
			pthread_mutex_destroy(&c->fork[i]);
	}
	pthread_mutex_destroy(&c->print_gate);
}

static void	philo_free_allocations(t_ctx *c)
{
	free(c->gate);
	c->gate = NULL;
	free(c->fork);
	c->fork = NULL;
	free(c->philo_data);
	c->philo_data = NULL;
}

void	handle_status_msg(char *prefix, char *name, char *message, int status)
{
	if (status)
	{
		if (prefix)
		{
			ft_putstr_fd(prefix, 2);
			ft_putstr_fd(": ", 2);
		}
		if (name)
		{
			ft_putstr_fd(name, 2);
			ft_putstr_fd(": ", 2);
		}
		if (message)
			ft_putstr_fd(message, 2);
		ft_putstr_fd("\n", 2);
	}
}

void	philo_exit(t_ctx *c, char *name, char *message, int status)
{
	handle_status_msg("philo", name, message, status);
	philo_destroy_mutexes(c);
	philo_free_allocations(c);
	exit(status);
}
