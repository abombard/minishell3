CC=gcc
FLAGS=-Wall -Wextra -Werror

NAME=ft_minishell3

LIBS_DIR=./libs
DIR_LIBFT=$(LIBS_DIR)/libft
DIR_PRINTF=$(LIBS_DIR)/ft_printf
DIR_LIST=$(LIBS_DIR)/list
DIR_CAPS=$(LIBS_DIR)/caps

LIBS=-L $(DIR_CAPS) -lcaps -L $(DIR_PRINTF) -lprintf -L $(DIR_LIBFT) -lft -L $(DIR_LIST) -llist

SRC_DIR=srcs
INCLUDES=-I ./includes

BUILD_DIR= __build

SRC=main.c\
	list_head.c\
	minishell__string_to_command_line.c\
	minishell__character_to_command_line.c\
	minishell__display_command_line.c\
	get_character_bytes_count.c\
	key__backspace.c\
	key__copy.c\
	key__cursor_to_begin_of_line.c\
	key__cursor_to_end_of_line.c\
	key__cursor_to_next_character.c\
	key__cursor_to_next_command.c\
	key__cursor_to_next_line.c\
	key__cursor_to_next_word.c\
	key__cursor_to_prev_character.c\
	key__cursor_to_prev_command.c\
	key__cursor_to_prev_line.c\
	key__cursor_to_prev_word.c\
	key__cut.c\
	key__cut_to_end_of_line.c\
	key__delete_command_line.c\
	key__delete_under_cursor.c\
	key__paste.c\
	key__select.c\
	key__send.c\
	key__share.c\

OBJ=$(addprefix $(BUILD_DIR)/,$(SRC:.c=.o))

all:$(BUILD_DIR) $(NAME)

$(BUILD_DIR):
	@mkdir -p $@

exec:
	@make -C $(DIR_LIBFT)
	@make -C $(DIR_PRINTF)
	@make -C $(DIR_LIST)
	@make -C $(DIR_CAPS)

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.c
	@$(CC) $(FLAGS) -c $< -o $@ $(INCLUDES)

$(NAME):exec $(OBJ)
	@$(CC) $(FLAGS) -o $@ $(OBJ) $(LIBS) $(INCLUDES) -ltermcap
	@echo "$@ was created"

clean:
	@rm -rf $(BUILD_DIR)

fclean: clean
	@rm -f $(NAME)
	@make $@ -C $(DIR_LIBFT)
	@make $@ -C $(DIR_PRINTF)
	@make $@ -C $(DIR_LIST)
	@make $@ -C $(DIR_CAPS)

re: fclean all
