NAME = philo
LIBFT = $(LIB_DIR)/libft.a

CC = gcc
CFLAGS = -Wall -Werror -Wextra #-g -fsanitize=thread

SRC_DIR = src/
OBJ_DIR = obj/
INC_DIR = include/
LIB_DIR = libft

SRCS =	check.c \
		initialize.c \
		life.c \
		main.c \
		utils.c

VPATH = $(SRC_DIR) $(INC_DIR) $(OBJ_DIR)

OBJS = $(SRCS:%.c=%.o)
OBJ_PRE = $(addprefix $(OBJ_DIR), $(OBJS))

DEP = philo.h
DEP_PRE = $(addprefix $(INC_DIR), $(DEP))


all: lib $(NAME)

$(OBJ_DIR)%.o: %.c $(DEP_PRE)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJ_PRE)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFT) $(RLINE) $(LFLAGS)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(RLINE)
	make clean -C $(LIB_DIR)

fclean: clean
	rm -rf $(NAME)
	make fclean -C $(LIB_DIR)

lib: 
	@make -C $(LIB_DIR)

re: fclean all

.PHONY: re debug fclean clean all run readline libfile