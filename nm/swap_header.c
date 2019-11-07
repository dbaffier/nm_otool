/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_header.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 23:45:40 by dbaffier          #+#    #+#             */
/*   Updated: 2019/10/31 20:03:14 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <mach-o/fat.h>

void	swap_fat_header(struct fat_header *hdr)
{
	hdr->magic = swap_uint32(hdr->magic);
	hdr->nfat_arch = swap_uint32(hdr->nfat_arch);
}

void	swap_fat_arch(struct fat_arch *arch, unsigned long n_fat_arch)
{
	unsigned long	i;

	i = 0;
	while (i < n_fat_arch)
	{
		arch[i].cputype = swap_uint32(arch[i].cputype);
		arch[i].cpusubtype = swap_uint32(arch[i].cpusubtype);
		arch[i].offset = swap_uint32(arch[i].offset);
		arch[i].size = swap_uint32(arch[i].size);
		arch[i].align = swap_uint32(arch[i].align);
		i++;
	}
}
