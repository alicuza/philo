_This project has been created as part of the 42 curriculum by sancuta._

# philosophers

## Description

`philosophers` is a solution for the "dining philosophers" problem which prints
out the state change of the different philosophers.

---

## Table of Contents

- [Description](#description)
- [Instructions](#instructions)
  - [Compilation](#compilation)
  - [Usage](#usage)
  - [Functionality](#functionality)
- [Known Limitations](#known-limitations)
- [Algorithm and Data Structure](#algorithm-and-data-structure)
  - [Architecture Overview](#architecture-overview)
  - [Core Data Structure](#core-data-structure)
  - [Concurrency](#concurrency)
    - [Threads](#threads)
    - [Mutexes](#mutexes)
    - [Atomics](#atomics)
    - [Deadlock Prevention](#deadlock-prevention)
  - [User Input](#user-input)
  - [Error Handling and Cleanup](#error-handling-and-cleanup)
- [Resources](#resources)
  - [Documentation and References](#documentation-and-references)
  - [AI Usage](#ai-usage)

---

## Instructions

### Compilation

- `make` or `make all` - compiles `philo`;
- `make clean` - removes object and dependency files;
- `make fclean` - additionally removes the binary;
- `make re` - recompiles the entire project from scratch;

### Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

All times are given in milliseconds. Without the optional fifth argument the
simulation runs until a philosopher dies.

### Functionality

Prints out all state changes of every philosopher in the following format:

```
timestamp_in_ms philo_nbr has taken a fork
timestamp_in_ms philo_nbr is eating
timestamp_in_ms philo_nbr is sleeping
timestamp_in_ms philo_nbr is thinking
timestamp_in_ms philo_nbr died
```

Timestamps are milliseconds since the simulation started, and `philo_nbr` is
1-indexed, so philosophers are numbered `1` to `number_of_philosophers`. The
death message is displayed within 10 ms of the actual death occurring, and no
further messages are printed after it.

---

## Known Limitations

- **Failure on tight timings.** When `time_to_die` is within a few milliseconds
  of `time_to_eat + time_to_sleep`, a philosopher has to reacquire both forks
  almost immediately after waking. With many philosophers competing for a few
  cores, scheduler jitter makes that unreliable and philosophers do starve. The
  following leaves 1 ms of slack against a 400 ms cycle floor:

```
./philo 198 401 200 200
```

- **Precision is costly.** To maintain accuracy, we have to periodically
  recheck the actual time during sleeping, not relying on `usleep` alone, which
  can be imprecise. This costs a nontrivial amount of CPU cycles, especially on
  the higher end of philo amounts.

---

## Algorithm and Data Structure

### Architecture Overview

The simulation runs one thread per philosopher and uses the main thread to
watch for starvation. Philosophers never coordinate with each other directly.
They contend for forks, and the only global decisions, when the simulation ends
and who died, are made by the main thread.

- **Philosopher threads.** One thread per philosopher, all created before the
  clock starts.
- **Start barrier.** An atomic flag releases every philosopher at once, so they
  all measure their timestamps from the same `t = 0`.
- **Forks.** One mutex per fork. A philosopher holds two of them while eating.
- **Output.** One mutex guards every printed line, so messages never interleave.
- **Simulation state.** One atomic counter holds how many philosophers have yet
  to finish, and doubles as the flag that stops the simulation.
- **Monitoring.** The main thread checks each philosopher in turn and ends the
  simulation as soon as one has gone too long without eating.

```
                     ┌───────────────────────────┐
                     │     create N threads      │
                     └───────────────────────────┘
                       │
                       ∨
                     ┌───────────────────────────┐
                     │      set start time       │
                     └───────────────────────────┘
                       │
                       ∨
┌──────────────┐     ┌───────────────────────────┐
│ monitor loop │ <── │    release start flag     │
└──────────────┘     └───────────────────────────┘
  │                    │
  │                    ∨
  │                  ┌───────────────────────────┐
  │                  │    wait on start flag     │
  │                  └───────────────────────────┘
  │                    │
  │                    ∨
  │                  ┌───────────────────────────┐
  │                  │        take forks         │ <┐
  │                  └───────────────────────────┘  │
  │                    │                            │
  │                    ∨                            │
  │                  ┌───────────────────────────┐  │
  │                  │            eat            │  │
  │                  └───────────────────────────┘  │
  │                    │                            │
  │                    ∨                            │
  │                  ┌───────────────────────────┐  │
  │                  │           sleep           │  │
  │                  └───────────────────────────┘  │
  │                    │                            │
  │                    ∨                            │
  │                  ┌───────────────────────────┐  │
  │                  │           think           │ ─┘
  │                  └───────────────────────────┘
  │                    │
  │                    ∨
  │                  ┌───────────────────────────┐
  └────────────────> │ stop, join, destroy, free │
                     └───────────────────────────┘
```

The two lanes run independently. Philosophers repeat their cycle until the
simulation stops, which happens when every philosopher has eaten the required
number of meals, or when one of them starves.

Without the optional meal count neither condition may ever be reached. If the
timings are survivable, nobody starves and there is nothing to count, so the
simulation runs until it is interrupted from the outside, for example with
`Ctrl-C`.

### Core Data Structure

Two structures. `t_ctx` belongs to the main thread and owns everything: the
argument values, the fork mutexes, the output mutex, and the bookkeeping needed
to tear it all down again. `t_thread_data` is what a single philosopher sees,
one per philosopher, holding its own state plus pointers into the context.

The program allocates two arrays, for one `pthread_mutex_t fork` and for one
`t_thread_data data` per philosopher. Nothing is copied: every philosopher
points at the same forks, the same output mutex and the same counters, which is
exactly what makes them contend.

Every field falls into one of five categories, and the category is what decides
whether it needs synchronising at all.

- **Private to one philosopher.** `meal_cnt` and `time_to_think` are written
  and read only by the thread that owns them, so they need no protection at
  all.
- **Read-only once the simulation starts.** `args`, `sim_start_time` and
  `philo_idx` are written during initialisation and never written again. This
  is only safe because of the start barrier: the main thread writes
  `sim_start_time` before releasing the flag and never touches it afterwards,
  and philosophers read it only after seeing the flag set, so the write is
  guaranteed to be visible to every one of them.
- **Shared and mutable.** `last_meal_time_ms`, `ph_to_go` and `start_flag` are
  written by one thread and read by another while both are running. All three
  are atomics.
- **The locks themselves.** The fork mutexes and the output mutex are not data
  that needs protecting, they are the protection. A philosopher reaches two of
  the forks through its `fork_first` and `fork_second` pointers, which are set
  once and from then on only ever name the same two mutexes.
- **Main thread only.** `tid` is written when a philosopher is created and read
  when it is joined; a philosopher never touches its own. `started`,
  `forks_ready` and `print_gate_ready` are separate: they record how far
  initialisation got, so that cleanup only undoes what was actually done.

### Concurrency

Three primitives do all the work: threads, mutexes and atomics. Each is used
for one job and nothing else.

#### Threads

- **One per philosopher, plus main.** Philosopher threads run the
  eat-sleep-think cycle; the main thread creates them, watches for starvation,
  and cleans up.
- **All created before the clock starts.** Creating a hundred threads takes
  measurable time, so none of them begin until every one exists.
- **Every thread is joined.** Nothing is detached, so the main thread knows all
  philosophers have stopped touching the forks before it destroys them.
- **The single philosopher is special.** With only one fork on the table it can
  never eat, so that thread takes its fork, announces it, and waits to die
  rather than looping.

#### Mutexes

- **One per fork.** A philosopher must hold two of them at once to eat, which
  is the entire contention in the problem.
- **One for output.** Every printed line is written while holding it, so lines
  from different philosophers never interleave. It also makes the death message
  final: the death is printed and the simulation stopped in the same critical
  section, so a philosopher already waiting for the lock will acquire it, find
  the simulation over, and return without printing.

#### Atomics

- **`last_meal_time_ms`.** Written by a philosopher when it starts eating, read
  by the main thread to decide whether it has starved.
- **`ph_to_go`.** Counts the philosophers that are still eating, and is forced
  to zero to stop the simulation, so it serves as both counter and stop flag.
- **`start_flag`.** Set once by the main thread to release every philosopher at
  the same moment.
- **Memory ordering.** An atomic guarantees a value is never read half-written,
  but it can also constrain the order of the operations around it. Relaxed
  ordering gives only the first guarantee. Release and acquire ordering pair up
  to give the second: everything written before a release store is visible to
  whoever reads that value with an acquire load.

`last_meal_time_ms` and `ph_to_go` are relaxed, because nothing else depends on
them. A philosopher reading a stale `ph_to_go` does one more iteration, and the
main thread reading a stale meal time notices the starvation a fraction of a
millisecond later.

`start_flag` is the one place ordering matters. The main thread writes
`sim_start_time` and then sets the flag with release ordering; philosophers read
the flag with acquire ordering before touching anything else. Downgrading either
side to relaxed would let a philosopher see the flag set and still read an
uninitialised start time.

#### Deadlock Prevention

- **What a deadlock is.** A set of threads each holding a resource and waiting
  for one held by another, so none of them can ever proceed. Here it happens if
  every philosopher picks up the fork on the same side at the same moment: every
  fork is then held, every philosopher waits for a second one, and nobody can
  release anything.
- **The approach.** Philosopher `i` uses forks `i` and `i + 1`, modulo the
  number of philosophers. Even-numbered philosophers take fork `i` first,
  odd-numbered ones take fork `i + 1` first. This creates contention on purpose:
  philosopher `i` and philosopher `i - 1` now both reach for fork `i` first,
  only one of them can win it, and the loser blocks while holding nothing at
  all. The forks are therefore never handed out one per philosopher, which is
  the distribution a deadlock needs.
- **Why it works.** Odd-numbered forks are never picked up first. An
  even-numbered philosopher starts with fork `i`, an odd-numbered one starts
  with fork `i + 1`, and both of those numbers are even. So anyone holding an
  odd-numbered fork must have taken it second, which means they already hold
  their first one. They have both forks, so they are eating rather than waiting,
  and will put both down shortly.

  A blocked philosopher is always waiting for an odd-numbered fork. It is
  therefore always waiting on somebody who is eating, not on somebody who is
  also stuck. The chain of waiting ends after one step instead of closing into a
  circle, and a circle is what a deadlock is.

  One wrinkle when the philosopher count is odd: the last philosopher's second
  fork wraps around to fork `0`, which is even. That makes the chain one step
  longer, because it can end up waiting on philosopher `0`. But philosopher `0`
  is waiting for fork `1`, which is odd, so the chain still terminates.

### User Input

- **Count.** Four arguments, or five with the optional meal count. Anything
  else is rejected.
- **Format.** Optional leading whitespace, an optional sign, optional leading
  zeros, then digits. Any other character anywhere, including trailing
  whitespace, rejects the argument.
- **Range.** Every value has to be a nonzero positive integer that fits in an
  unsigned 32 bit integer, so `1` to `UINT_MAX`.

Passing validation does not guarantee the simulation can run. A philosopher
count near `UINT_MAX` is a valid integer, but needs more memory for its forks
than the machine has, and fails at the next stage instead:

```
$ ./philo 4294967295 800 200 200
philo: alloc_simulation: Cannot allocate memory
```

### Error Handling and Cleanup

The init functions are grouped by failure type, so a message names both what
went wrong and where. Steps that cannot fail, such as `init_start_flag` and
`assign_data_to_philo`, return `void`.

- **`validate_args`.** The arguments themselves.
- **`alloc_simulation`.** Every allocation in the program, so the only stage
  that can run out of memory.
- **`init_mutexes`.** The fork mutexes and the output mutex.
- **`start_philos`.** Creating the threads, and on a real machine the stage that
  actually fails, when the process hits its thread limit.

pthread functions return their error code instead of setting `errno`, so each
stage passes that code up and the caller renders it with `strerror`. Any failure
exits with status `1`.

Cleanup takes one path whether the simulation finished or a stage failed: stop
the simulation, release the start flag so a philosopher still waiting to begin
wakes and sees the stop, join every thread that was **actually** created,
destroy the mutexes that were **actually** initialised, free the two arrays. The
context records how far initialisation got, because freeing memory that live
threads still read, or joining a thread that was never created, turns a failed
startup into a crash or a hang.

---

## Resources

### Documentation and References

- [`pthreads(7)`](https://man7.org/linux/man-pages/man7/pthreads.7.html) - overview of the POSIX threads API;
- [`pthread_create(3)`](https://man7.org/linux/man-pages/man3/pthread_create.3.html) - thread creation, and the return convention pthread uses instead of `errno`;
- [`pthread_mutex_lock(3)`](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3.html) - locking, unlocking, and the error codes they return;
- [POSIX `<stdatomic.h>`](https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/stdatomic.h.html) - atomic types and the explicit-ordering functions;
- [POSIX Memory Ordering and Synchronization](https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap04.html#tag_04_15) - section 4.15, what acquire and release actually guarantee;
- Butenhof, D. R. (1997). *Programming with POSIX Threads* - threading model, mutex usage, and the chapter on common pitfalls;
- Bryant, R. E. & O'Hallaron, D. R. (2011). *Computer Systems: A Programmer's Perspective* - chapter 12 on concurrent programming;

### AI Usage

AI tools were used mainly for interactive rubber ducking and searching for
information.
