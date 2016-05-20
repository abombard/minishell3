/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps__init_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/09 15:49:10 by abombard          #+#    #+#             */
/*   Updated: 2016/05/09 15:49:14 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_caps.h"
#include "types.h"
#include "log.h"

#include "libft.h"

bool	caps__init_func(const char *in_tcapcode, bool (*in_func)())
{
	t_internal_context	*context;
	unsigned int		i;

	if (!in_tcapcode || !in_func)
		FATAL("in_tcapcode %p in_func %p", \
				(void *)in_tcapcode, (void *)in_func);
	caps__get_context(&context);
	i = 0;
	while (i < context->map_size)
	{
		if (context->map[i].tcapcode)
		{
			if (!ft_strcmp(in_tcapcode, context->map[i].tcapcode))
			{
				context->map[i].func = in_func;
				return (TRUE);
			}
		}
		i++;
	}
	return (FALSE);
}
