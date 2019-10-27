/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_char_conv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 22:42:55 by dbaffier          #+#    #+#             */
/*   Updated: 2019/06/11 05:52:05 by mmonier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	get_char_conv(t_format *data)
{
	static	t_convert	converter[] = {CONVERT_C, CONVERT_S, CONVERT_D,
		CONVERT_P, CONVERT_I, CONVERT_O, CONVERT_U, CONVERT_X, CONVERT_GF,
		CONVERT_F, CONVERT_G, CONVERT_GX, CONVERT_GC, CONVERT_GS, CONVERT_GX,
		CONVERT_B, CONVERT_R, CONVERT_GR, CONVERT_K, CONVERT_GU, CONVERT_GD,
		CONVERT_GO};
	size_t				i;

	i = 0;
	while (i < sizeof(converter) / sizeof(converter[0]))
	{
		if (converter[i].type == *data->tail)
			return (converter[i].type);
		i++;
	}
	return ('\0');
}
