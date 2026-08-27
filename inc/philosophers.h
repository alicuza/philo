/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:20 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/27 22:14:12 by sancuta          ###   ########.fr       */
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
# include <stdatomic.h>

/* ----- argument definitions ----------------------------------------------- */
# define ARG_START_IDX		1
# define MIN_ARG_CNT		4
# define MAX_ARG_CNT		5

// TODO: consider whether these should be an enum
# define NBR_PHILOS		0
# define T_DIE			1
# define T_EAT			2
# define T_SLEEP		3
# define NBR_MEALS		4

/* ----- state change strings ----------------------------------------------- */
#define FORK	"has taken a fork"
#define EAT		"is eating"
#define SLEEP	"is sleeping"
#define THINK	"is thinking"
#define DEAD	"died"

/*
 *	struct timeval
 *	{
 *		time_t		tv_sec;		// seconds
 *		suseconds_t	tv_usec;	// microseconds
 *	};
 *	we need miliseconds: 1 second = 1.000 miliseconds = 1.000.000 microseconds
 */
typedef struct timeval	t_timeval;

typedef void	*(t_routine)(void *);

typedef struct s_thread_data
{
	atomic_int64_t	last_meal_time_ms;
	atomic_int64_t	*ph_to_go;
	t_timeval		*simulation_start_time;
	uint32_t		*args;
	pthread_mutex_t	*gate;
	pthread_mutex_t	*print_gate;
	pthread_mutex_t	*fork_first;
	pthread_mutex_t	*fork_second;
	pthread_t		tid;
	uint32_t		philo_idx;
	uint32_t		meal_cnt;
}	t_thread_data;

typedef struct	s_ctx
{
	uint32_t		args[MAX_ARG_CNT];
	t_timeval		sim_start_time;
	t_thread_data	*philo_data;
	pthread_mutex_t	*fork;
	pthread_mutex_t	*gate;
	pthread_mutex_t	print_gate;
	atomic_int64_t	ph_to_go;
}	t_ctx;

/* ----- validation_utils.c ------------------------------------------------- */
size_t		sig_digits_strlen(const char *nbr);
int64_t		ft_atol(const char *nbr);

/* ----- init_simulation.c -------------------------------------------------- */
void		init_context(t_ctx *c, int argc, char **argv);

/* ----- philo_helpers.c ---------------------------------------------------- */
bool		is_simulation_running(t_thread_data *data);
int64_t		get_time_in_ms(t_timeval *start, t_timeval *end);
size_t		ft_strlen(const char *s)

/* ----- routines.c --------------------------------------------------------- */
void		*routine_single_philo(void *arg);
void		*routine_multiple_philos(void *arg);

/* ----- print_helpers.c ---------------------------------------------------- */
bool		init_print(t_thread_data *data, uint32_t *idx, int64_t *time);
int			print_formatted(int64_t time, uint32_t idx, char *action);
bool		print_action(t_thread_data *data, char *action);
bool		print_eat(t_thread_data *data);
void		ft_putstr_fd(char *c, int fd);

/* ----- philo_destroy_mutexes.c -------------------------------------------- */
void		philo_exit(t_ctx *c, char *name, char *message, int status);

/* ----- exit_cleanup.c ----------------------------------------------------- */
void		handle_status_msg(char *prefix, char *name, char *message, int status);
void		philo_exit(t_ctx *c, char *name, char *message, int status);
#endif
