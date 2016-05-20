#include "key.h"
#include "minishell.h"
#include "list_head.h"
#include "types.h"
#include "log.h"

bool	key__cut_to_end_of_line(t_internal_context *in_context)
{
	size_t	selection_start;
	size_t	selection_end;

	if (in_context->state != STATE_REGULAR)
		return (TRUE);
	selection_start = in_context->command_line.offset;
	selection_end = in_context->command_line.size - in_context->command_line.offset;
	key__share__cut_build_copy(in_context,
										 selection_start,
										 selection_end);
	return (TRUE);
}
