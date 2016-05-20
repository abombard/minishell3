#include "key.h"
#include "minishell.h"
#include "list_head.h"
#include "types.h"
#include "log.h"

bool	key__cursor_to_next_word(t_internal_context *in_context)
{
	key__share__next_word_offset(&in_context->command_line,
										   in_context->command_line.offset,
										   &in_context->command_line.offset);
	return (TRUE);
}
