NAME	=	codexion
CC	=	cc
CFLAGS	=	-Wall -Wextra -Werror -I include

SRC	=	./src/main.c \
		./src/parsing.c \
		./src/ft_split.c \
		./src/helpers.c \
		./src/protocol.c

OBJ = $(SRC:.c=.o)
all = $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONEY: all clean fclean re
