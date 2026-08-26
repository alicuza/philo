/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:20 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/25 21:28:11 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <unistd.h>		// write, usleep
# include <stdio.h>			// printf
# include <string.h>		// memset
# include <stdlib.h>		// malloc, free
# include <sys/time.h>		// gettimeofday
# include <pthread.h>		// pthread_create, pthread_detach, pthread_join,
							// pthread_mutex_init, pthread_mutex_destroy,
							// pthread_mutex_lock, pthread_mutex_unlock
# include <limits.h>
# include <stdint.h>
# include <stdbool.h>

/* ----- fork indices relative to each philosopher -------------------------- */
# define F_LEFT 0
# define F_RIGHT 1

/* ----- argument definitions ----------------------------------------------- */
# define ARG_START_IDX 1
# define MIN_ARG_CNT 4
# define MAX_ARG_CNT 5

// TODO: consider whether these should be an enum
# define NBR_PHILOS 0
# define T_DIE 1
# define T_EAT 2
# define T_SLEEP 3
# define NBR_MEALS 4

/* ----- argument validation flags ------------------------------------------ */
#define VALID 0
#define INVALID_ARG_CNT 1
#define INVALID_NMB 2
#define INVALID_RNG 3

/* ----- state change strings ----------------------------------------------- */
#define FORK "has taken a fork"
#define EAT "is eating"
#define SLEEP "is sleeping"
#define THINK "is thinking"
#define DEAD "died"

/*
 *	struct timeval
 *	{
 *		time_t		tv_sec;		// seconds
 *		suseconds_t	tv_usec;	// microseconds
 *	};
 *	we need miliseconds: 1 second = 1.000 miliseconds = 1.000.000 microseconds
 */
typedef struct timeval	t_timeval;

typedef struct s_thread_data
{
	t_timeval		last_meal_time;
	t_timeval		last_action_time;
	t_timeval		*simulation_start_time;
	uint32_t		philo_idx;
	uint32_t		*args;
	pthread_t		tid;
	bool			dead;	// TODO: needs to be initialized somewhere
	pthread_mutex_t	*gate;
	pthread_mutex_t	*print_gate;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
}	t_thread_data;

typedef struct	s_ctx
{
	pthread_mutex_t	*fork;
	pthread_mutex_t	*gate;
	t_thread_data	*philo_data;
	pthread_mutex_t	print_gate;
	t_timeval		simulation_start_time;	// TODO: does this need to be in ctx?
	uint32_t		args[MAX_ARG_CNT];	// TODO: i might need to add this to the philo args, so i can decided if a philo is the last one or not
}	t_ctx;

/* ----- string_utils.c ----------------------------------------------------- */
size_t	sig_digits_strlen(const char *nbr);
int		ft_isspace(int c);
int		ft_issign(int c);
int		ft_isdigit(int c);
int		printf_mutex(t_thread_data *data, char *time_to_what);

/* ----- init_simulation.c -------------------------------------------------- */
void	init_context(t_ctx *c, int argc, char **argv);
void	start_simulation(t_ctx *c);

/* ----- time_utils.c ------------------------------------------------------- */
int64_t	get_time_in_ms(t_timeval *start, t_timeval *end);

/* ----- routine.c ---------------------------------------------------------- */
void	*routine(void *arg);

/* ----- ft_atol.c ---------------------------------------------------------- */
int64_t	ft_atol(const char *nbr);
#endif
