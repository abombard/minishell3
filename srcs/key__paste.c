#include "minishell.h"
#include "list_head.h"
#include "types.h"
#include "log.h"

bool	key__paste(t_internal_context *in_context)
{
	t_list		*pos;
	t_list_head	dup;

	if (in_context->state == STATE_REGULAR)
	{
		pos = list_nth(&in_context->command_line.list, in_context->command_line.offset - 1);
		list_head__command_line_dup(&dup, &in_context->copy);
		list_splice(&dup.list, pos);
		in_context->command_line.size += dup.size;
		in_context->command_line.offset += dup.size;
	}
	return (TRUE);
}
