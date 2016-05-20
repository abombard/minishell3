/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps__init_func_by_keycode.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/09 15:50:07 by abombard          #+#    #+#             */
/*   Updated: 2016/05/09 15:51:59 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_caps.h"
#include "types.h"
#include "log.h"

#include <termcap.h>

bool	caps__init_func_by_keycode(const t_buffer in_keycode, bool (*in_func)())
{
	t_internal_context	*context;
	size_t				i;

	if (in_keycode.size == 0 || !in_keycode.bytes)
		FATAL("in_keycode.size %zu in_keycode.bytes %p",
				in_keycode.size, (void *)in_keycode.bytes);
	caps__get_context(&context);
	i = 0;
	while (i < context->map_size)
	{
		if (!caps__keycode_cmp(in_keycode, context->map[i].keycode))
		{
			context->map[i].func = in_func;
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}
