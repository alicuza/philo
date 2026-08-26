# journal

- [todo](#todo)
- [philosophers](#philosophers)
- [perosnal](#personal)
- [documentation](#documentation)
  - [online](#online)
  - [local](#local)

## todo

- [p] play around with pthreads and mutexes
- [p] read the man pages for the allowed external functions
- [ ] vendor the required libft functions
- [ ] prepare the Makefile template
- [ ] figure out what other research needs to be done
- [ ] have a working prototype
- [ ] iterate and test
- [x] ~~prepare the readme template~~
- [x] ~~download the assignment~~
- [x] ~~prepare the header file with the necessary includes for the allowed external functions~~
- [x] ~~prepare the `.gitignore` file~~

## philosophers

**2026.08.17.**
- sketch readme with some basic info
- start the journal
- write header file with the necessary `.h` files for allowed functions

**2026.08.19.**
- simple concurrency example, using `pthread_create` and `pthread_join`
- experiment with data races, by using the following code:

```c
#include <stdio.h>
#include <pthread.h>

#define PHILO_COUNT 14
#define ROUTINE_ITERATION_COUNT 1000

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
```

**2026.08.20**
- write `.gitignore`
- explore `pthread_detach` - doesn't need to be reaped for its resource to be freed
- read Butenhof
- check `prototype.c` and `prototype_detach.c` with `valgrind`: the tool returns leaks when threads are detached, because the main thread is returning without waiting for the detached threads to be done, so of course the resources are not freed; adding an appropriate `usleep` amount before returning fixes valgrind's output; the proper way would be to use `pthread_exit` in `main`, making the main thread wait for all other threads to finish before returning

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./prototype_detach
```


## personal

**2026.08.17**
- `restrict` keyword -> is a compile-time optimization hint, this pointer is the sole accessor of the object pointed to. Violating it is undefined behaviour ([C99 §6.7.3.1]).
- `_Nullable` keyword -> compiler annotation, counterpart to `_Nonnull`

**2026.08.19.**
- read from the C99 standard: types, especially `size_t` and `uintptr_t`, and how `va_args` demands the actual type you need (can be cast afterward).

**2026.08.20**
- add errata on practice problem 12.2 in [Computer Systems: A Programer's Perspective]
- call `waitpid((pid_t)-1, NULL, WNOHANG)` to not block if no children terminated yet

### documentation

#### online

#### local

**man pages**

**pdfs**
[C99 Standard Committee Draft N1256](literature/C99_standard_n1256.pdf)
[Butenhof - Programming with POSIX Threads](literature/Butenhof_\(1997\)_Programming_with_POSIX_Threads.djvu)
[Bryant, O'Hallaron Computer Systems: A Programmer's Perspective](literature/Bryant&OHallaron_\(2011\)-Computer_Systems_A_Programmers_Perspective.pdf)

