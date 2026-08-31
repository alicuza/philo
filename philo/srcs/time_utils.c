/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 21:02:21 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/31 21:20:24 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	sleep_until(t_timeval *start_time, int64_t deadline_ms)
{
	t_timeval	cur_time;
	int64_t		remaining_ms;

	while (true)
	{
		gettimeofday(&cur_time, NULL);
		remaining_ms = deadline_ms - get_time_in_ms(start_time, &cur_time);
		if (remaining_ms <= 0)
			return ;
		if (remaining_ms > 500)
			usleep(400000);
		else if (remaining_ms > 5)
			usleep((remaining_ms - 5) * 1000);
		else
			usleep(50);
	}
}

int64_t	get_time_in_ms(t_timeval *start, t_timeval *end)
{
	int64_t	time_in_ms;

	time_in_ms = (end->tv_sec - start->tv_sec) * 1000
		+ (500 + end->tv_usec - start->tv_usec) / 1000;
	return (time_in_ms);
}
