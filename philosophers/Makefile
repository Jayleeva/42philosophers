NAME = philosophers

SRC_DIR = ./src
SRC =	main.c \
		ft_atoi.c \
		simulation.c \

SRC := $(addprefix $(SRC_DIR)/, $(SRC))

OBJ_DIR = ./obj
$(OBJ_DIR):
	mkdir $(OBJ_DIR)
OBJ := $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I ./inc

all: $(NAME)

$(NAME): $(OBJ)
	@${CC} ${CFLAGS} -o $(NAME) $(addprefix $(OBJ_DIR)/, $(OBJ))

clean:
	rm -f $(addprefix $(OBJ_DIR)/, $(OBJ)) -r $(OBJ_DIR)
fclean: clean
	rm -f $(NAME)
re: fclean all

.PHONY : all clean fclean re