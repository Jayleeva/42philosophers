NAME = philosophers

SRC_DIR = ./src
SRC =	main.c \
		init.c \
		utils.c \
		simulation.c \
		actions.c \
		end_conditions.c \

SRC := $(addprefix $(SRC_DIR)/, $(SRC))

OBJ := $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I ./inc

all: $(NAME)

$(NAME): $(OBJ)
	@${CC} ${CFLAGS} -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ) -r
fclean: clean
	rm -f $(NAME)
re: fclean all

.PHONY : all clean fclean re