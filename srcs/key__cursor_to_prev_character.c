#include "minishell.h"
#include "list_head.h"
#include "types.h"
#include "log.h"

bool	key__cursor_to_prev_character(t_internal_context *in_context)
{
	if (in_context->command_line.offset <= PROMPT_SIZE)
		return (TRUE);
	in_context->command_line.offset -= 1;
	return (TRUE);
}
