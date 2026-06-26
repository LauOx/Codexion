NAME          = codexion
CC            = cc
CFLAGS        = -Wall -Wextra -Werror

# Folder
DIR_OBJ       = obj/

# Flags to include .h
INCLUDES      = -I include

# Archivos
SRCS          = include/codex.h \
				src/coders.c \
				src/coders-vitals.c\
				src/init.c \
				src/main.c \
				src/parser.c \
				src/queue.c \
				src/simulation.c \
				src/utils.c \

# Generates: obj/src/coders.o etc
OBJS          = $(SRCS:%.c=$(DIR_OBJ)%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Compilation rule
$(DIR_OBJ)%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(DIR_OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re