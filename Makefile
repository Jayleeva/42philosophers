NAME = philosophers

SRC_DIR = ./src
SRC = 	main.c \
		ft_atoi.c \
		simulation.c \
		actions.c \
		free_list.c \
		
OBJ = $(SRC:.c=.o)

SRC := $(addprefix $(SRC_DIR)/, $(SRC))

CC = cc
CFLAGS = -Wall -Wextra -Werror -I ./inc

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_LIB)
	@${CC} ${CFLAGS} ${OBJ} $(LIBFT_LIB) -o $(NAME)

clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all

.PHONY : all clean fclean re