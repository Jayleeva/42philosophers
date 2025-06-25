NAME = philosophers

SRC_DIR = ./src
OBJ_DIR = ./obj

SRC =   main.c \
        init.c \
        utils.c \
        simulation.c \
        actions.c \
        end_conditions.c

OBJ = $(SRC:.c=.o)

SRCS := $(addprefix $(SRC_DIR)/, $(SRC))
OBJS := $(addprefix $(OBJ_DIR)/, $(OBJ))

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I ./inc

MKDIR = mkdir -p
RM = rm -rf

all: $(NAME)

$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)

$(NAME): $(OBJS) | $(OBJ_DIR)
	@${CC} ${CFLAGS} -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@$(RM) $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY : all clean fclean re