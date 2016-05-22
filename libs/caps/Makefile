CC=gcc
FLAGS=-Wall -Wextra -Werror

NAME=libcaps.a

LIBS_DIR=./libs
DIR_LIBFT=$(LIBS_DIR)/libft
DIR_PRINTF=$(LIBS_DIR)/ft_printf

LIBS=-L $(DIR_LIBFT) -lft -L $(DIR_PRINTF) -lprintf

SRC_DIR=./srcs
INCLUDES=-I ./includes

BUILD_DIR= __build

SRC=caps__context.c\
	caps__initialize.c\
	caps__finalize.c\
	caps__init_func.c\
	caps__init_func_by_keycode.c\
	caps__exec_func.c\
	caps__keycode_dump.c\
	caps__keycode_cmp.c\
	caps__keycode_find.c\
	caps__win.c\
	caps__print.c\
	caps__print_cap.c\
	caps__delete_line.c\
	caps__cursor_to_offset.c\
	caps__cursor_getxy.c\
	caps__cursor_setxy.c

OBJ=$(addprefix $(BUILD_DIR)/,$(SRC:.c=.o))

all:$(NAME)

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.c
	@$(CC) $(FLAGS) -c $< -o $@ $(INCLUDES)

$(NAME):$(BUILD_DIR) $(OBJ)
	@ar rc $@ $(OBJ)
	@ranlib $@
	@echo "$@ was created"

test:$(NAME)
	@make -C $(DIR_LIBFT)
	@make -C $(DIR_PRINTF)
	@$(CC) $(FLAGS) -o test test.c $^ $(LIBS) $(INCLUDES) -ltermcap
	@echo "$@ was created"

clean:
	@rm -rf $(BUILD_DIR)

fclean: clean
	@make $@ -C $(DIR_LIBFT)
	@make $@ -C $(DIR_PRINTF)
	@rm -f $(NAME) test

re: fclean all
