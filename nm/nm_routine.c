/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 00:28:07 by dbaffier          #+#    #+#             */
/*   Updated: 2019/10/27 02:01:03 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <mach-o/fat.h>
#include <ar.h>


static uint32_t	ft_fat(t_nm *nm_t, void *file)
{
	size_t				i;
	size_t				iter;
	unsigned int		cpu;
	struct fat_header	*hdr;
	struct fat_arch		*arch;

	hdr = (struct fat_header *)file;
	arch = (struct fat_arch *)(file + sizeof(struct fat_header));
	iter = hdr->magic == FAT_MAGIC ? hdr->nfat_arch : swap_uint32(hdr->nfat_arch);
	nm_t->file_type = OFILE_FAT;
	i = 0;
	while (i < iter)
	{
		cpu = (hdr->magic == FAT_MAGIC) ? arch[i].cputype : swap_uint32(arch[i].cputype);
		if (cpu == CPU_TYPE_X86_64)
			return (hdr->magic == FAT_MAGIC ? arch[i].offset : swap_uint32(arch[i].offset));
		i++;
	}
	return (0);
}

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
