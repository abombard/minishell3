#include "internal_caps.h"
#include "libft.h"
#include "log.h"

#define ANSI_Q_CURSORXY			"\033[6n"
#define ANSI_Q_CURSORXY_SIZE	 sizeof("\033[6n") - 1

bool	caps__cursor_getxy(int *x, int *y)
{
	char	buf[16];
	size_t	i;

	*x = 0;
	*y = 0;
	if (write(1, ANSI_Q_CURSORXY, ANSI_Q_CURSORXY_SIZE) != ANSI_Q_CURSORXY_SIZE)
		FATAL("write() failed %s", "");
	if (read(0, buf, sizeof(buf)) == sizeof(buf))
		FATAL("buf too small %zu", (size_t)sizeof(buf));
	i = sizeof("\033[") - 1;
	*x = ft_atoi(buf + i);
	while (ft_isdigit(buf[i]))
		i++;
	i += sizeof(";") - 1;
	*y = ft_atoi(buf + i);
	return (TRUE);
}

