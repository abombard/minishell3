/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::	  */
/*	 main.c												:+:		 :+:	:+:	  */
/*													  +:+ +:+		  +:+	  */
/*	 By: abombard <marvin@42.fr>					+#+	 +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2016/03/31 16:58:07 by abombard		   #+#	  #+#			  */
/*   Updated: 2016/05/09 16:01:35 by abombard         ###   ########.fr       */
/*																			  */
/* ************************************************************************** */

#include "caps.h"
#include "types.h"
#include "log.h"

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <termcap.h>

# define KEY_ESC		0x1b
# define KEY_BS			0x7f
# define KEY_ENTER		0x0b

# define INPUT_BUFFER_SIZE_MAX	7

static bool minishell__read_input(int fd)
{
	size_t			input_buffer_size;
	char			input_buffer[INPUT_BUFFER_SIZE_MAX];

	if (!isatty(fd))
		FATAL("isatty() failed fd %d", fd);

	while (1)
	{
		input_buffer_size = read(fd, input_buffer, sizeof (input_buffer));
		input_buffer[input_buffer_size] = 0;

		LOG_DEBUG("key: (size: %zu) %s\r", input_buffer_size, caps__keycode_dump(input_buffer_size, input_buffer));

		if (input_buffer[0] == KEY_ESC || input_buffer[0] == KEY_BS ||
			(input_buffer_size == 1 && input_buffer[0] < 27))
			caps__keycode_find(input_buffer_size, input_buffer);
		else
		{
			/* save command line */
		}

		if (input_buffer[0] == 'q')
			break ;
	}

	return (TRUE);
}

static bool minishell__termios_raw(int fd)
{
	struct termios			termios_old;
	struct termios			termios_new;

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

	/* set terminal in raw mode */
	if (tcsetattr(fd, TCSAFLUSH, &termios_new) != 0)
		FATAL("tcsetattr() failed to initialize %s", "");

	if (!minishell__read_input(fd))
	{
		LOG_ERROR("minishell__read_input() failed %s", "");

		/* set terminal back to normal */
		if (tcsetattr(fd, TCSADRAIN, &termios_old) != 0)
			LOG_ERROR("tcsetattr() failed t finalize %s", "");

		return (FALSE);
	}

	/* set terminal back to normal */
	if (tcsetattr(fd, TCSADRAIN, &termios_old) != 0)
		FATAL("tcsetattr() failed t finalize %s", "");

	return (TRUE);
}

# define TTY_DEVICE "/dev/tty"

static bool minishell__device_open(void)
{
	int		fd;

	/* open a tty */
	fd = open(TTY_DEVICE, O_RDONLY);
	if (fd == -1)
		FATAL("open() failed on %s", TTY_DEVICE);

	LOG_DEBUG("our program read and write on %s fd %d", TTY_DEVICE, fd);

	if (!minishell__termios_raw(fd))
	{
		LOG_ERROR("minishell__termios_init() failed %s", "");

		if (close(fd))
			LOG_ERROR("close() failed on %s", TTY_DEVICE);
		return (FALSE);
	}

	if (close(fd))
		FATAL("close() failed on %s", TTY_DEVICE);

	return (TRUE);
}

#define ASSERT(expr) if (!expr) FATAL("(%s) failed", #expr)

static bool minishell__initialize_key_map(void)
{
	return (TRUE);
}

int main()
{
	if (!caps__initialize())
	{
		LOG_ERROR("caps_initialize() failed %s", "");
		return (1);
	}

	if (!minishell__initialize_key_map())
	{
		LOG_ERROR("minishell__initialize_key_map() failed %s", "");
		return (1);
	}

	if (!minishell__device_open())
	{
		LOG_ERROR("minishell__raw_mode() failed %s", "");
		return (1);
	}

	if (!caps__finalize())
	{
		LOG_ERROR("caps_finalize() failed %s", "");
		return (1);
	}

	return (0);
}
