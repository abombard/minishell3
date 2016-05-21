#include "key.h"
#include "minishell.h"
#include "list_head.h"
#include "caps.h"
#include "types.h"
#include "log.h"

//TEMP
#include "ft_printf.h"
#include "libft.h"

#include <stdlib.h>

bool	key__send(t_internal_context *context)
{
	size_t	buffer_size_max;
	size_t	buffer_size;
	char	*buffer;

	if (context->state == STATE_REGULAR)
	{
		if (!minishell__display_command_line(&context->command_line))
			FATAL("minishell__display_command_line() failed %s\r", "");
		caps__print_cap(CAPS__DOWN, 0);
		caps__print_cap(CAPS__CARRIAGE_RETURN, 0);
		if (context->command_line.size > PROMPT_SIZE)
		{
			/* -- command line to buffer */
			buffer_size_max = context->command_line.size * 4;
			/* malloc */
			buffer = (char *)malloc(buffer_size_max + 1);
			if (!buffer)
				FATAL("malloc() failed command line buffer_size_max %zu", buffer_size_max);
			if (!list_head__command_line_to_buffer(&context->command_line, buffer_size_max, &buffer_size, buffer))
			{
				free(buffer);
				FATAL("list_head__command_line_to_buffer() failed %s\r", "");
			}
			buffer[buffer_size] = '\0';
			ft_printf("Command line: %s\r\n\r", buffer);

			/* exit */
			if (!ft_strcmp(buffer + PROMPT_SIZE, "exit"))
				context->state = STATE_EXIT;
			/* free */
			free(buffer);

			if (!key__share__command_line_to_history(context))
				FATAL("key__share__command_line_to_history() failed %s\r", "");
			context->history.offset = context->history.size;
			if (!minishell__string_to_command_line(PROMPT_SIZE, PROMPT, &context->command_line))
				FATAL("minishell__string_to_command_line() failed %s", PROMPT);
		}
	}
	return (TRUE);
}
