#include "key.h"
#include "minishell.h"
#include "list_head.h"
#include "types.h"
#include "log.h"

bool	key__copy(t_internal_context *in_context)
{
	size_t	copy_start;
	size_t	copy_size;

	if (in_context->state == STATE_SELECTION)
	{
		in_context->state = STATE_REGULAR;
		if (!key__share__selection_get(in_context, &copy_start, &copy_size))
			FATAL("key__share__selection_get() failed %s\r", "");
		if (!key__share__copy_build_copy(&in_context->command_line, copy_start, copy_size, &in_context->copy))
			FATAL("key__share__copy_build_copy() failed %s\r", "");
	}
	return (TRUE);
}
