/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps__print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/09 15:53:57 by abombard          #+#    #+#             */
/*   Updated: 2016/05/21 19:10:52 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_caps.h"
#include <unistd.h>
#include <termcap.h>

static int	internal_caps__putc(int c)
{
	return (write(1, &c, 1));
}

#include "log.h"
extern int	caps__print(const char *s, const size_t line_count)
{
	if (ospeed < 0)
	{
		LOG_WARNING("ospeed %d", ospeed);
		ospeed = 300;
	}
	if (!s)
	{
		tputs("(Null)", 1, &internal_caps__putc);
		return (0);
	}
	return (tputs(s, line_count, &internal_caps__putc));
}
