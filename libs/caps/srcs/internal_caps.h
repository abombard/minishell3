/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::	  */
/*	 internal_caps.h									:+:		 :+:	:+:	  */
/*													  +:+ +:+		  +:+	  */
/*	 By: abombard <marvin@42.fr>					+#+	 +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2016/04/01 15:58:22 by abombard		   #+#	  #+#			  */
/*	 Updated: 2016/04/01 19:29:50 by abombard		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#ifndef INTERNAL_CAPS_H
# define INTERNAL_CAPS_H

# include "../caps.h"
# include "types.h"
# include <unistd.h>

# define CAPS__MAP_KEY_COUNT		144
# define CAPS__CAP_COUNT			9

typedef struct	s_key_map
{
	char		*tcapcode;
	t_buffer	keycode;
	char		*description;
	bool		(*func)();
}				t_key_map;

typedef struct	s_internal_context
{
	char		*termtype;
	char		termbuffer[2048];
	char		*buffaddr;

	size_t		map_size;
	t_key_map	map[CAPS__MAP_KEY_COUNT];

	size_t		caps_size;
	char		*caps[CAPS__CAP_COUNT];
}				t_internal_context;

/*
** getter for caps
*/
extern void		caps__get_context (t_internal_context **context);

#endif
