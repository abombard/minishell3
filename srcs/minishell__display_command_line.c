#include "minishell.h"
#include "libft.h"
#include "types.h"
#include "log.h"

#include <stdlib.h>
#include <caps.h>
/*
** Display the command line
*/
bool	minishell__display_command_line(const t_list_head *command_line)
{
	size_t	buffer_size_max;
	size_t	buffer_size;
	char	*buffer;

	buffer_size_max = command_line->size * CHARACTER_SIZE_MAX;
	buffer = malloc(buffer_size_max);
	if (!buffer)
		FATAL("malloc() failed command_line->size %zu", buffer_size_max);
	if (!list_head__command_line_to_buffer(command_line,
										   buffer_size_max,
										   &buffer_size,
										   buffer))
	{
		LOG_ERROR("list_head__command_line_to_buffer() %s\r", "");
		free(buffer);
		return (FALSE);
	}
	write(1, buffer, buffer_size);
	if (buffer_size % caps__win(WIN_COLUMNS) == 0)
	{
		caps__print_cap(CAPS__DOWN, 0);
		caps__print_cap(CAPS__CARRIAGE_RETURN, 0);
	}
	free(buffer);
	return (TRUE);
}
