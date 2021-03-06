/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps__delete_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/09 15:51:24 by abombard          #+#    #+#             */
/*   Updated: 2016/05/21 18:18:55 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_caps.h"

void	caps__delete_line(const size_t line_offset)
{
	size_t	col_count;
	size_t	line_count;
	size_t	i;

	caps__print_cap(CAPS__CARRIAGE_RETURN, 0);
	col_count = caps__win(WIN_COLUMNS);
	if (col_count != 0)
	{
		line_count = line_offset / col_count;
		i = 0;
		while (i < line_count)
		{
			caps__print_cap(CAPS__UP, 0);
			i++;
		}
		caps__print_cap(CAPS__CLEAR_TO_END_OF_SCREEN, caps__win(WIN_COLUMNS));
	}
}
