#include "caps.h"
#include "minishell.h"
#include "list_head.h"
#include "types.h"
#include "log.h"

bool	key__cursor_to_next_line(t_internal_context *in_context)
{
	int	distance;

	distance = in_context->command_line.offset + caps__win(WIN_COLUMNS);
	if (distance < (int)in_context->command_line.size)
		in_context->command_line.offset = distance;
	else
		in_context->command_line.offset = in_context->command_line.size;
	return (TRUE);
}
