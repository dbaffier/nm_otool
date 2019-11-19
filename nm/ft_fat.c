/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 01:58:23 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/14 13:48:07 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ofile.h"
#include <mach-o/fat.h>

uint32_t	process_fat(t_prg *prg, t_ofile *of)
{
	unsigned long		i;

	(void)prg;
	i = 0;
	while (i < of->fat_header->nfat_arch)
	{
		if (of->fat_archs[i].cputype == CPU_TYPE_X86_64)
			return (of->fat_archs[i].offset);
		i++;
	}
	return (0);
}

int	ft_fat(t_ofile *of, void *addr, uint32_t magic, enum byte_sex e)
{
	(void)e;
	if (of->file_size >= sizeof(struct fat_header) && (magic == FAT_MAGIC
					|| swap_uint32(magic) == FAT_MAGIC))
	{
		of->object_addr = addr;
		of->file_type = OFILE_FAT;
		of->fat_header = (struct fat_header *)addr;
		of->fat_archs = (struct fat_arch *)(addr + sizeof(struct fat_header));
		(magic == FAT_MAGIC) ? 0 : swap_fat_header(of->fat_header);
		(magic == FAT_MAGIC) ? 0 : swap_fat_arch(of->fat_archs, of->fat_header->nfat_arch);
		// print_fat_headers(of->fat_header, of->fat_archs, of->file_size);
	}
	return (0);
}
