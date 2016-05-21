/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::	  */
/*	 caps__initialize.c									:+:		 :+:	:+:	  */
/*													  +:+ +:+		  +:+	  */
/*	 By: abombard <marvin@42.fr>					+#+	 +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2016/04/01 17:20:28 by abombard		   #+#	  #+#			  */
/*	 Updated: 2016/04/01 17:25:07 by abombard		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#include "internal_caps.h"
#include "types.h"
#include "log.h"
#include "libft.h"

#include <stdlib.h>
#include <termcap.h>

static bool	internal_caps__initialize_caps(t_internal_context *caps)
{
	char	*capcodes[] = {
		"nd", "le", "up", "do", "dc", "ce", "cd", "cr", "bc"
	};
	size_t	i;

	caps->caps_size = sizeof(caps->caps) / sizeof(caps->caps[0]);
	i = 0;
	while (i < caps->caps_size)
	{
		caps->caps[i] = tgetstr(capcodes[i], &caps->buffaddr);
		if (i != caps->caps_size - 1 && !caps->caps[i])
		{
			LOG_ERROR("tgetstr() failed on %s", capcodes[i]);
		}
		else if (i == caps->caps_size - 1)
		{
			caps->caps[i] = "\b";
		}
		i++;
	}
	return (TRUE);
}

static bool	internal_caps__initialize_keycodes(t_internal_context *caps)
{
	char		*keycode;
	size_t		i;

	caps->map_size = sizeof(caps->map) / sizeof(caps->map[0]);
	i = 0;
	while (i < caps->map_size)
	{
		if (caps->map[i].tcapcode)
		{
			keycode = tgetstr(caps->map[i].tcapcode, &caps->buffaddr);
			if (keycode)
			{
				caps->map[i].keycode.bytes = keycode;
				caps->map[i].keycode.size = ft_strlen(keycode);
				//LOG_DEBUG("Successfully initialized %s", caps->map[i].description);
			}
			//else
			//LOG_WARNING("Could not find %s", caps->map[i].description);
		}
		i++;
	}
	return (TRUE);
}

#include <termios.h>
static bool	internal_caps__tgetent(t_internal_context *caps)
{
	char		*termtype;
	char		*temp;

	termtype = CAPS__TERMTYPE;
	if (!termtype)
		FATAL("getenv(\"TERM\") failed %s", "");
	caps->termtype = termtype;
	if (!tgetent(caps->termbuffer, termtype))
		FATAL("tgetent() failed termtype %s", termtype);
	caps->buffaddr = caps->termbuffer;
	/* For termcaps functions */
	temp = tgetstr("pc", &caps->buffaddr);
	PC = temp ? *temp : 0;
	BC = tgetstr("le", &caps->buffaddr);
	UP = tgetstr("up", &caps->buffaddr);
	return (TRUE);
}

bool		caps__initialize(void)
{
	t_internal_context	*caps;

	caps__get_context(&caps);
	if (!internal_caps__tgetent(caps))
		FATAL("interal_caps__tgetent() failed %s", "");
	if (!internal_caps__initialize_keycodes(caps))
		FATAL("internal_caps__initialize_keycodes() failed %s", "");
	if (!internal_caps__initialize_caps(caps))
		FATAL("internal_caps__initialize_caps() failed %s", "");
	return (TRUE);
}
