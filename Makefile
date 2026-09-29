NAME	=	codexion
CC	=	cc
CFLAGS	=	-Wall -Wextra -Werror -pthread -g -I include

SRC	=	./src/main.c \
		./src/parsing.c \
		./src/ft_split.c \
		./src/helpers.c \
		./src/protocol.c \
		./src/monitor.c \
		./src/protection.c

OBJ = $(SRC:.c=.o)
all = $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(NAME)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONEY: all clean fclean re
