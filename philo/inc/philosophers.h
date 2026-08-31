/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:34:20 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/27 22:50:13 by sancuta          ###   ########.fr       */
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
# include <errno.h>

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
# define FORK	"has taken a fork"
# define EAT		"is eating"
# define SLEEP	"is sleeping"
# define THINK	"is thinking"
# define DEAD	"died"

/* ----- error messages ----------------------------------------------------- */
# define E_ARGS		"Expected 4-5 positive integers up to UINT_MAX"
# define E_ALLOC	"Cannot allocate memory"
# define E_MUTEX	"Could not initialise a mutex"
# define E_THREAD	"Could not create a thread"

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
	atomic_int_least64_t	last_meal_time_ms;
	atomic_int_least64_t	*ph_to_go;
	t_timeval				*sim_start_time;
	uint32_t				*args;
	atomic_bool				*start_flag;
	pthread_mutex_t			*print_gate;
	pthread_mutex_t			*fork_first;
	pthread_mutex_t			*fork_second;
	pthread_t				tid;
	uint32_t				philo_idx;
	uint32_t				meal_cnt;
	uint32_t				time_to_think;
	int64_t					last_print_time_ms;
}	t_thread_data;

typedef struct s_ctx
{
	uint32_t				args[MAX_ARG_CNT];
	t_timeval				sim_start_time;
	t_thread_data			*philo_data;
	pthread_mutex_t			*fork;
	atomic_bool				start_flag;
	pthread_mutex_t			print_gate;
	atomic_int_least64_t	ph_to_go;
	uint32_t				started;
	uint32_t				forks_ready;
	bool					print_gate_ready;
}	t_ctx;

/* ----- validation_utils.c ------------------------------------------------- */
size_t		sig_digits_strlen(const char *nbr);
int64_t		ft_atol(const char *nbr);

/* ----- init_context.c ----------------------------------------------------- */
int			init_context(t_ctx *c, int argc, char **argv);

/* ----- init_alloc.c ------------------------------------------------------- */
int			alloc_simulation(t_ctx *c);

/* ----- init_sync.c -------------------------------------------------------- */
int			init_mutexes(t_ctx *c);
void		init_start_flag(t_ctx *c);
void		wait_for_start(t_thread_data *data);
bool		is_simulation_running(t_thread_data *data);

/* ----- init_threads.c ----------------------------------------------------- */
int			start_philos(t_ctx *c);

/* ----- time_utils.c ------------------------------------------------------- */
void		sleep_until(t_timeval *start_time, int64_t deadline_ms);
int64_t		get_time_in_ms(t_timeval *start, t_timeval *end);

/* ----- string_utils.c ----------------------------------------------------- */
size_t		ft_strlen(const char *s);
void		ft_putstr_fd(char *c, int fd);

/* ----- fork_utils.c ------------------------------------------------------- */
bool		guard_lock(atomic_int_least64_t *ph_to_go, pthread_mutex_t *mutex);
bool		take_both_forks(t_thread_data *data);
void		drop_forks(t_thread_data *data);

/* ----- routines.c --------------------------------------------------------- */
void		*routine_single_philo(void *arg);
void		*routine_multiple_philos(void *arg);

/* ----- print_utils.c ------------------------------------------------------ */
int			print_formatted(int64_t time, uint32_t idx, char *action);
bool		print_action(t_thread_data *data, char *action);
bool		print_eat(t_thread_data *data);

/* ----- exit_cleanup.c ----------------------------------------------------- */
void		handle_status_msg(char *prefix, char *name, char *msg, int status);
int			philo_exit(t_ctx *c, char *name, char *msg, int status);
#endif
