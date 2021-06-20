NAME = "ft_ls"

SRC = main.c \
myexit.c \
sorting.c \
tree.c \
tree_balance.c \
tree_print.c

OBJ = $(SRC:.c=.o)

INCLUDES = ./

FLAGS = #-Wall -Wextra -Werror

all: $(NAME)

$(NAME):
# 	gcc $(FLAGS) $(SRC) -L./libft -lft
	gcc *.c  -L./libft -lft -o $(NAME)

.c.o:
	gcc $(FLAGS) -c $<