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
  - [User Input](#user-input)
  - [Error Handling and Cleanup](#error-handling-and-cleanup)
- [Resources](#resources)
  - [Documentation and References](#documentation-and-references)
  - [AI Usage](#ai-usage)

---

## Instructions

### Compilation

- `make` or `make all` - compiles `philosophers`;
- `make clean` - removes object and dependency files;
- `make fclean` - additionally removes the binary, and docs;
- `make re` - recompiles the entire project from scratch;
- `make debug` - compiles with `-g` for debugging;
- `make test` - builds release binary, then runs the test suite;
- `make doc` - generates Doxygen man pages in `docs/` (requires `doxygen`);
- `make compile_flags.txt` - generates a clangd-compatible compilation database;

### Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Functionality

Prints out all state changes of a every philosopher in the following format:

```
timestamp_in_ms philo_nbr has taken a fork
timestamp_in_ms philo_nbr is eating
timestamp_in_ms philo_nbr is sleeping
timestamp_in_ms philo_nbr is thinking
timestamp_in_ms philo_nbr died
```

The death message is displayed withing 10 ms of the actual death occuring.

---

## Known Limitations

- all sorts of pthread bs.

---

## Algorithm and Data Structure

### Architecture Overview

### User Input

---

## Resources

### Documentation and References

### AI Usage

AI tools were used mainly for interactive rubber ducking and searching for
information.
