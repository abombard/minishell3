/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps__keycode_cmp.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/09 15:50:19 by abombard          #+#    #+#             */
/*   Updated: 2016/05/09 15:50:19 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log.h"
#include "libft.h"

int			caps__keycode_cmp(const t_buffer keycode1, const t_buffer keycode2)
{
	size_t	size;
	size_t	i;

	if (keycode1.size != keycode2.size)
		return (keycode1.size - keycode2.size);
	size = keycode1.size;
	if (size > 2)
		i = 2;
	else
		i = 0;
	while (i < size)
	{
		if (keycode1.bytes[i] != keycode2.bytes[i])
			break ;
		i++;
	}
	if (i == size)
		return (0);
	return (keycode1.bytes[i] - keycode2.bytes[i]);
}
