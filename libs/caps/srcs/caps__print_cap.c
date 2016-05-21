/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps__print_cap.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/09 15:49:48 by abombard          #+#    #+#             */
/*   Updated: 2016/05/21 14:46:00 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_caps.h"
#include "types.h"
#include "log.h"

void				caps__print_cap(const t_caps_cap value, int lines)
{
	t_internal_context	*context;

	caps__get_context(&context);
	caps__print(context->caps[value], lines);
}
