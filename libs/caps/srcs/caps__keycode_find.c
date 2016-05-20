/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::	  */
/*	 caps__keycode_find.c								:+:		 :+:	:+:	  */
/*													  +:+ +:+		  +:+	  */
/*	 By: abombard <marvin@42.fr>					+#+	 +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2016/04/01 17:53:14 by abombard		   #+#	  #+#			  */
/*	 Updated: 2016/04/01 17:54:18 by abombard		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "types.h"
#include "log.h"
#include "internal_caps.h"

bool	caps__keycode_find(const size_t in_keycode_size, const char *in_keycode)
{
	t_internal_context	*context;
	t_buffer			keycode;
	size_t				i;

	if (in_keycode_size == 0 || in_keycode == NULL)
		FATAL("in_keycode_size %zu in_keycode %p", in_keycode_size, in_keycode);
	keycode.size = in_keycode_size;
	keycode.bytes = (char *)in_keycode;
	caps__get_context(&context);
	i = 0;
	while (i < context->map_size)
	{
		if (!caps__keycode_cmp(keycode, context->map[i].keycode))
		{
			LOG_DEBUG("You typed %s\r", context->map[i].description);
			return (TRUE);
		}
		i++;
	}
	LOG_DEBUG("Could not find the code %s\r", caps__keycode_dump(in_keycode_size, in_keycode));
	return (FALSE);
}
