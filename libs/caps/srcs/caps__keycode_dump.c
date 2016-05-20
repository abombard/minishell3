/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps__keycode_dump.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/09 15:50:46 by abombard          #+#    #+#             */
/*   Updated: 2016/05/09 15:54:55 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log.h"
#include "types.h"
#include "libft.h"

static bool	internal__keycode_dump(const size_t keycode_size,
									const char *keycode,
									const size_t dump_size,
									char *dump)
{
	char			*hex;
	unsigned char	c;
	size_t			i;
	size_t			j;

	hex = "0123456789abcdef";
	i = 0;
	j = 0;
	while (i < keycode_size)
	{
		if (j + 3 > dump_size)
			FATAL("buffer overflow %s\r", "");
		if (i != 0)
			dump[j++] = ':';
		c = keycode[i];
		dump[j + 1] = hex[c & 0xf];
		dump[j] = hex[(c >> 4) & 0xf];
		j += 2;
		i++;
	}
	dump[j] = 0;
	return (TRUE);
}

char		*caps__keycode_dump(const size_t keycode_size, const char *keycode)
{
	static char		dump[2][64];
	static size_t	dump_index = 0;

	if (!keycode)
	{
		LOG_ERROR("keycode %p", (void *)keycode);
		return (NULL);
	}
	dump_index = dump_index == (sizeof(dump) / sizeof(dump[0]) - 1) ?
		0 : dump_index + 1;
	if (!internal__keycode_dump(keycode_size,
								keycode,
								sizeof(dump[0]),
								dump[dump_index]))
	{
		LOG_ERROR("internal_caps__keycode_dump() failed keycode %s\r", keycode);
		return (NULL);
	}
	return (dump[dump_index]);
}
