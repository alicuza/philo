# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/23 17:28:50 by sancuta           #+#    #+#              #
#    Updated: 2026/08/23 17:42:05 by sancuta          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ---- project ------------------------------------------------------------- #
NAME       = philo

# ---- compiler / linker --------------------------------------------------- #
CC         = cc
CFLAGS     = -Wall -Wextra -Werror -MMD -MP
CPPFLAGS   = -I inc
LDFLAGS    =
LDLIBS     = -lpthread

# ---- build directories --------------------------------------------------- #
RELEASE_DIR = build

# ---- sources ------------------------------------------------------------- #
SRCS        = \
			main.c \
			init_context.c \
			init_alloc.c \
			init_sync.c \
			init_threads.c \
			routines.c \
			validation_utils.c \
			print_utils.c \
			time_utils.c \
			string_utils.c \
			exit_cleanup.c

RELEASE_OBJS = $(addprefix $(RELEASE_DIR)/, $(SRCS:.c=.o))

# ---- source lookup ------------------------------------------------------- #
vpath %.c srcs

# ---- build flags --------------------------------------------------------- #
RELEASE_FLAGS = -O2

# ---- targets ------------------------------------------------------------- #
all: $(NAME)

$(NAME): $(RELEASE_OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# ---- object compilation -------------------------------------------------- #
$(RELEASE_DIR)/%.o: BUILD_FLAGS = $(RELEASE_FLAGS)

$(RELEASE_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(BUILD_FLAGS) -c $< -o $@

# ---- utility targets ----------------------------------------------------- #
clean:
	rm -rf build

fclean: clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

# ---- phony targets ------------------------------------------------------- #
.PHONY: all clean fclean re

# ---- dependencies -------------------------------------------------------- #
-include $(RELEASE_OBJS:.o=.d)
