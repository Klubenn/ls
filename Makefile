NAME = ft_ls

LIBFT_DIR = ./libft/
LIBFT = lft
PRINTF_DIR = ./libft/ft_printf/
PRINTF = lftprintf
LIBFTPRINTF = ./libft/ft_printf/libftprintf.a
LIBFTLIB = ./libft/libft.a

INCL_DIR = ./includes/
INCL_H = ls.h
INCL = $(addprefix $(INCL_DIR),$(INCL_H))

SRC_DIR = src/

SRC_C = absent_arguments.c \
		gather_file_data.c \
		input_parsing.c \
		main.c \
		myexit.c \
		sorting.c \
		tree.c \
		tree_balance.c \
		tree_print.c

SRC = $(addprefix $(SRC_DIR),$(SRC_C))
SRC_O = $(SRC:%.c=%.o)


FLAGS = -Wall -Wextra -Werror

all: $(LIBFT_DIR)$(LIBFT) $(NAME)

$(LIBFT_DIR)$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(SRC) $(INCL) $(LIBFTLIB) $(LIBFTPRINTF)
	gcc $(FLAGS) $(SRC) -I$(INCL_DIR) -L$(LIBFT_DIR) -$(LIBFT) -L$(PRINTF_DIR) -$(PRINTF) -o $(NAME)

clean:
	make clean -C $(LIBFT_DIR)
	rm -rf $(SRC_O)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean flcean re