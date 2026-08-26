#include <stdio.h>
#include <pthread.h>

#define PHILO_COUNT 5
#define ROUTINE_ITERATION_COUNT 100

void	*routine(void *arg);

int main(void)
{
	pthread_t	philo[PHILO_COUNT];
	int			x;
	size_t		i;

	x = 0;
	i = -1;
	while(++i < PHILO_COUNT)
		pthread_create(&philo[i], NULL, routine, &x);
	i = -1;
	while(++i < PHILO_COUNT)
		pthread_join(philo[i], NULL);

	return (0);
}

void	*routine(void *arg)
{
	size_t	i;

	i = -1;
	while (++i < ROUTINE_ITERATION_COUNT)
		++*(int *)arg;
	printf("philo says x = %d!\n", *(int *)arg);
	return (NULL);
}
