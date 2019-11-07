/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 00:28:07 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/04 22:26:18 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <mach-o/fat.h>
#include <ar.h>


int		nm_routine(t_nm *nm_t, void *addr)
{
	uint32_t	magic;

	(void)nm_t;
	magic = *(uint32_t *)addr;
	if (magic == FAT_MAGIC || magic == FAT_CIGAM)
	{
		addr += ft_fat(t_nm *nm_t, addr);
		magic = *(uint32_t *)addr;
	}
	ft_printf("%#x\n", magic);
	return (0);
}
