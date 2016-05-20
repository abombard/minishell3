#include "types.h"
#include "log.h"
#include "internal_caps.h"

bool	caps__exec_func(const size_t in_keycode_size,
						const char *in_keycode,
						void *in_context)
{
	t_internal_context	*context;
	t_buffer			keycode;
	size_t				i;

	keycode.size = in_keycode_size;
	keycode.bytes = (char *)in_keycode;
	caps__get_context(&context);
	i = 0;
	while (i < context->map_size)
	{
		if (!caps__keycode_cmp(keycode, context->map[i].keycode))
		{
			if (context->map[i].func)
			{
				if (!context->map[i].func(in_context))
					return (FALSE);
			}
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}
