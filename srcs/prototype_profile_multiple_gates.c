#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>

#define PHILO_COUNT 200

typedef struct	s_data
{
	pthread_t		tid;
	pthread_mutex_t	gate;
	pthread_mutex_t	*print_gate;
	struct timeval	p_time;
	struct timeval	*start_time;
	size_t			idx;
}	t_data;

void	*routine(void *arg);

int main(void)
{
	t_data	philo[PHILO_COUNT];
	pthread_mutex_t	print_gate;
	struct timeval	start_time;
	struct timeval	end_time;
	size_t	i;

	memset(philo, 0, sizeof(philo));
	pthread_mutex_init(&print_gate, NULL);
	i = -1;
	while(++i < PHILO_COUNT)
	{
		pthread_mutex_init(&philo[i].gate, NULL);
		pthread_mutex_lock(&philo[i].gate);
		philo[i].start_time = &start_time;
		philo[i].print_gate = &print_gate;
		philo[i].idx = i;
		pthread_create(&philo[i].tid, NULL, routine, &philo[i]);
	}
	gettimeofday(&start_time, NULL);
	i = -1;
	while(++i < PHILO_COUNT)
		pthread_mutex_unlock(&philo[i].gate);
	gettimeofday(&end_time, NULL);
	pthread_mutex_lock(&print_gate);
	printf("after unlocking\n sec = %lu\nusec = %lu\n", end_time.tv_sec - start_time.tv_sec, end_time.tv_usec - start_time.tv_usec);
	pthread_mutex_unlock(&print_gate);
	i = -1;
	while(++i < PHILO_COUNT)
		pthread_join(philo[i].tid, NULL);
	return (0);
}

void	*routine(void *arg)
{
	t_data	*philo;

	philo = (t_data *)(arg);
	pthread_mutex_lock(&philo->gate);
	pthread_mutex_unlock(&philo->gate);
	gettimeofday(&philo->p_time, NULL);
	pthread_mutex_lock(philo->print_gate);
	printf("philo[%lu]\n sec = %lu\nusec = %lu\n", philo->idx, philo->p_time.tv_sec - philo->start_time->tv_sec, philo->p_time.tv_usec - philo->start_time->tv_usec);
	pthread_mutex_unlock(philo->print_gate);
	return (NULL);
}
