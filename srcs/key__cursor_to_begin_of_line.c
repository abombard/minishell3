#include "minishell.h"
#include "list_head.h"
#include "types.h"
#include "log.h"

bool	key__cursor_to_begin_of_line(t_internal_context *in_context)
{
	in_context->command_line.offset = PROMPT_SIZE;
	return (TRUE);
}
