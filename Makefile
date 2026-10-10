NAME	=	codexion
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror -pthread -I include
OBJ_DIR	=	./obj

SRC		=	./src/main.c \
			./src/init.c \
			./src/parsing.c \
			./src/protocol.c \
			./src/monitor.c \
			./src/dongles.c \
			./src/queue_funcs.c \
			./src/helpers/ft_split.c \
			./src/helpers/getters.c \
			./src/helpers/time_utils.c \
			./src/helpers/setters.c \
			./src/helpers/pending.c \
			./src/helpers/queue_helpers.c \
			./src/helpers/parsing_helpers.c \
			./src/helpers/utils.c

OBJ		=	$(SRC:./src/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: ./src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
