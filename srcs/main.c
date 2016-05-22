#include "caps.h"
#include "types.h"
#include "log.h"
#include "libft.h"
#include "ft_printf.h"
#include "list_head.h"
#include "minishell.h"
#include "key.h"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

static t_internal_context context = {
	.state = STATE_REGULAR,

	.command_line = {
		.size = 0,
		.list = {
			.next = &context.command_line.list,
			.prev = &context.command_line.list
		},
		.offset = 0
	},

	.copy = {
		.size = 0,
		.list = {
			.next = &context.copy.list,
			.prev = &context.copy.list
		},
		.offset = 0
	},
	.selection_offset_start = 0,
	.selection_offset_end = 0,

	.history = {
		.size = 0,
		.list = {
			.next = &context.history.list,
			.prev = &context.history.list
		},
		.offset = 0
	},
};

# define ISUTF(c) (((c) & 0xc0) != 0x80)

bool	isunicode(const char c, size_t *out_character_bytes_count)
{
	size_t		character_missing_bytes_count;

	get_character_bytes_count(1, &c, &character_missing_bytes_count);
	if (ISUTF(c) && character_missing_bytes_count > 0 && character_missing_bytes_count < 4)
	{
		*out_character_bytes_count = character_missing_bytes_count;
		return (TRUE);
	}
	return (FALSE);
}

/*
** Read and treat raw input
*/
# define INPUT_SIZE_MAX	7

typedef enum {
	MINISHELL__INPUT_TYPE_UNDEFINED = 0,
	MINISHELL__INPUT_TYPE_PRINT,
	MINISHELL__INPUT_TYPE_UNICODE,
	MINISHELL__INPUT_TYPE_CAPS
} t_input_type;

/*
** Identify
*/
static bool	minishell__identify_input(const unsigned int c, t_input_type *input_type, size_t *input_size_missing)
{
	size_t	character_size;

	*input_type = MINISHELL__INPUT_TYPE_UNDEFINED;
	*input_size_missing = 0;
	if (ft_isprint(c))
	{
		*input_type = MINISHELL__INPUT_TYPE_PRINT;
	}
	else if (isunicode(c, &character_size))
	{
		*input_type = MINISHELL__INPUT_TYPE_UNICODE;
		*input_size_missing = character_size;
	}
	else
	{
		*input_type = MINISHELL__INPUT_TYPE_CAPS;
		if (c == '\x1b')
			*input_size_missing = INPUT_SIZE_MAX - 1;
	}
	return (TRUE);
}

/*
** Treat
*/
static bool	minishell__treat_input(const t_input_type input_type,
					   const size_t input_buffer_size,
					   const char *input_buffer,
					   t_internal_context *context)
{
	if (input_type == MINISHELL__INPUT_TYPE_PRINT)
	{
		minishell__string_to_command_line(input_buffer_size, input_buffer, &context->command_line);
	}
	else if (input_type == MINISHELL__INPUT_TYPE_CAPS)
	{
		caps__exec_func(input_buffer_size, input_buffer, context);
	}
	else
	{
		ft_printf("\n\rAvoid Unicode\n\r");
	}

	return (TRUE);
}

#define CURSOR_A_GET_XY_SIZE		(sizeof("\033") - 1)
#define CURSOR_A_GET_XY				"\033"

#define CURSOR_Q_GET_XY_SIZE		(sizeof("\033[6n") - 1)
#define CURSOR_Q_GET_XY				"\033[6n"

/*
** Read loop
*/
static bool minishell__read_input(const int fd)
{
	size_t			input_buffer_size;
	char			input_buffer[INPUT_SIZE_MAX];
	t_input_type		input_type;
	size_t			input_size_missing;

	/* First PROMPT */
	ASSERT(minishell__string_to_command_line(PROMPT_SIZE, PROMPT, &context.command_line));
	ASSERT(minishell__display_command_line(&context.command_line));

	/* Read input loop */
	while (context.state != STATE_EXIT)
	{
		input_buffer_size = read(fd, input_buffer, 1);
		ASSERT(input_buffer_size == 1);

		minishell__identify_input(input_buffer[0], &input_type, &input_size_missing);
		if (input_buffer_size + input_size_missing > sizeof(input_buffer))
			FATAL("Buffer overflow on imput_buffer, need %zu but have %zu", input_buffer_size + input_size_missing, sizeof(input_buffer));

		if (input_size_missing)
			input_buffer_size += read(fd, input_buffer + 1, input_size_missing);

		//ft_printf("\n\rkeycode: %s\n\r", caps__keycode_dump(input_buffer_size, input_buffer));


		caps__delete_line(context.command_line.offset);

		minishell__treat_input(input_type,
				       input_buffer_size,
		     		       input_buffer,
				       &context);

		ASSERT(minishell__display_command_line(&context.command_line));

		caps__cursor_to_offset(context.command_line.offset, context.command_line.size);
	}
	return (TRUE);
}

/*
** Put the terminal in POSIX raw mode
*/
#include <termcap.h>
static bool minishell__termios_raw(const int fd)
{
	struct termios			termios_new;
	struct termios			termios_old;

	if (!isatty(fd))
		FATAL("We're not reading on a tty fd %d", fd);

	/* get the current termios structure */
	if (tcgetattr(0, &termios_old) != 0)
		FATAL("tcgetattr() failed fd_old %d", fd);

	termios_new = termios_old;

	/* POSIX raw */
	termios_new.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
				 | INLCR | IGNCR | ICRNL | IXON);
	termios_new.c_oflag &= ~OPOST;
	termios_new.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	termios_new.c_cflag &= ~(CSIZE | PARENB);
	termios_new.c_cflag |= CS8;
	termios_new.c_cc[VMIN] = 1;
	termios_new.c_cc[VTIME] = 0;

	long termios_ospeed = cfgetospeed(&termios_old);
	ospeed = 10000 / termios_ospeed;
	LOG_DEBUG("termios_ospeed %ld ospeed %d", termios_ospeed, ospeed);
	//cfsetospeed(&termios_new, ospeed);

	/* set terminal in raw mode */
	if (tcsetattr(fd, TCSANOW, &termios_new) != 0)
		FATAL("tcsetattr() failed to initialize %s\r", "");

	if (!minishell__read_input(fd))
	{
		LOG_ERROR("minishell__read_input() failed %s\r", "");

		/* set terminal back to normal */
		if (tcsetattr(fd, TCSADRAIN, &termios_old) != 0)
			LOG_ERROR("tcsetattr() failed t finalize %s\r", "");

		return (FALSE);
	}

	/* set terminal back to normal */
	if (tcsetattr(fd, TCSADRAIN, &termios_old) != 0)
		FATAL("tcsetattr() failed t finalize %s\r", "");

	return (TRUE);
}

/*
** Open a tty
*/
# define TTY_DEVICE "/dev/tty"

static bool minishell__device_open(void)
{
	int		fd;

	/* open a tty */
	fd = open(TTY_DEVICE, O_RDONLY);
	if (fd == -1)
		FATAL("open() failed on %s\r", TTY_DEVICE);

	if (!minishell__termios_raw(fd))
	{
		LOG_ERROR("minishell__termios_init() failed %s\r", "");

		if (close(fd))
			LOG_ERROR("close() failed on %s\r", TTY_DEVICE);
		return (FALSE);
	}

	if (close(fd))
		FATAL("close() failed on %s\r", TTY_DEVICE);

	return (TRUE);
}

/*
** Initialize key functions
*/
static bool minishell__initialize_key_map(void)
{
	/*
	** Emacs like
	*/
	ASSERT(caps__init_func_by_keycode(CAPS__KEYCODE_META_M, &key__send));
	ASSERT(caps__init_func_by_keycode(CAPS__KEYCODE_META_Q, &key__delete_command_line));
	ASSERT(caps__init_func_by_keycode(CAPS__KEYCODE_META_D, &key__delete_under_cursor));
	ASSERT(caps__init_func_by_keycode(CAPS__KEYCODE_META_A, &key__cursor_to_begin_of_line));
	ASSERT(caps__init_func_by_keycode(CAPS__KEYCODE_META_E, &key__cursor_to_end_of_line));
	ASSERT(caps__init_func_by_keycode(CAPS__KEYCODE_META_SPACE, &key__select));
	ASSERT(caps__init_func_by_keycode(CAPS__KEYCODE_META_W, &key__cut));
	ASSERT(caps__init_func_by_keycode(CAPS__KEYCODE_META_X, &key__copy));
	ASSERT(caps__init_func_by_keycode(CAPS__KEYCODE_META_Y, &key__paste));
	ASSERT(caps__init_func_by_keycode(CAPS__KEYCODE_META_K, &key__cut_to_end_of_line));
	ASSERT(caps__init_func_by_keycode(CAPS__KEYCODE_META_CURSOR_LEFT, &key__cursor_to_prev_word));
	ASSERT(caps__init_func_by_keycode(CAPS__KEYCODE_META_CURSOR_RIGHT, &key__cursor_to_next_word));
	ASSERT(caps__init_func_by_keycode(CAPS__KEYCODE_BACKSPACE, &key__backspace));


	/*
	** Legit
	*/
	ASSERT(caps__init_func(DELETE_UNDER_CURSOR_KEY, &key__delete_under_cursor));
	ASSERT(caps__init_func(CURSOR_LEFT_KEY, &key__cursor_to_prev_character));
	ASSERT(caps__init_func(CURSOR_RIGHT_KEY, &key__cursor_to_next_character));
	ASSERT(caps__init_func(CURSOR_UP_KEY, &key__cursor_to_prev_command));
	ASSERT(caps__init_func(CURSOR_DOWN_KEY, &key__cursor_to_next_command));
	ASSERT(caps__init_func(SHIFTED_CURSOR_LEFT_KEY, &key__cursor_to_prev_word));
	ASSERT(caps__init_func(SHIFTED_CURSOR_RIGHT_KEY, &key__cursor_to_next_word));
	ASSERT(caps__init_func(SCROLLING_BACKWARD_KEY, &key__cursor_to_prev_line));
	ASSERT(caps__init_func(SCROLLING_FORWARD_KEY, &key__cursor_to_next_line));

	ASSERT(caps__init_func(CURSOR_HOME_KEY, &key__cursor_to_begin_of_line));
	ASSERT(caps__init_func(CURSOR_HOME_DOWN_KEY, &key__cursor_to_end_of_line));
	ASSERT(caps__init_func(BEGIN_KEY, &key__cursor_to_begin_of_line));
	ASSERT(caps__init_func(END_KEY, &key__cursor_to_end_of_line));
	ASSERT(caps__init_func(MARK_KEY, &key__select));
	ASSERT(caps__init_func(SELECT_KEY, &key__select));
	ASSERT(caps__init_func(COPY_KEY, &key__copy));
	ASSERT(caps__init_func(ENTER_KEY, &key__send));

	return (TRUE);
}

/*
** Main
*/
int main()
{
	if (!caps__initialize())
	{
		LOG_ERROR("caps_initialize() failed %s\r", "");
		return (1);
	}
	if (!minishell__initialize_key_map())
	{
		LOG_ERROR("minishell__initialize_key_map() failed %s\r", "");
		return (1);
	}
	if (!minishell__device_open())
	{
		LOG_ERROR("minishell__raw_mode() failed %s\r", "");
		return (1);
	}
	if (!caps__finalize())
	{
		LOG_ERROR("caps_finalize() failed %s\r", "");
		return (1);
	}

	/* Free */
	list_head__command_line_destroy(&context.copy);
	list_head__command_line_destroy(&context.command_line);
	list_head__history_destroy(&context.history);

	return (0);
}
