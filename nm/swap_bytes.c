/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_bytes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 23:55:58 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/16 23:24:24 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ofile.h"

uint16_t swap_uint16(uint16_t val) 
{
    return (val << 8) | (val >> 8);
}

int16_t swap_int16(int16_t val) 
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

uint32_t swap_uint32(uint32_t val)
{
 //   val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF); 
   // return (val  << 16) | (val >> 16);
	return (((val & 0xFF00FF00) >> 8) | ((val & 0xFF00FF) << 8));
}


int32_t swap_int32(int32_t val)
{
	return (((val & 0xFF00FF00) >> 8) | ((val & 0xFF00FF) << 8));
}
