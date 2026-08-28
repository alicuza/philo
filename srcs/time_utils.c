/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:02:21 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/28 17:41:17 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	precise_sleep(int64_t time_ms)
{
	t_timeval	start_time;
	t_timeval	cur_time;

	gettimeofday(&start_time, NULL);
	while (true)
	{
		gettimeofday(&cur_time, NULL);
		if (get_time_in_ms(&start_time, &cur_time) >= time_ms)
			return ;
		usleep(200);
	}
}

int64_t	get_time_in_ms(t_timeval *start, t_timeval *end)
{
	int64_t	time_in_ms;

	time_in_ms = (end->tv_sec - start->tv_sec) * 1000
		+ (end->tv_usec - start->tv_usec) / 1000;
	return (time_in_ms);
}
