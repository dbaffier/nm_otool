/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_fat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 23:21:23 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/11 18:57:19 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ofile.h"
#include <mach-o/fat.h>

void	print_fat_headers(struct fat_header *hdr,
		struct fat_arch *arc, unsigned long size)
{
	long		i;

	i = -1;
	ft_printf("Fat headers\n");
	ft_printf("fat magic %#x\n", (unsigned long)(hdr->magic));
	ft_printf("nfat_arch %lu", hdr->nfat_arch);
	ft_printf("\n");
	while (++i < hdr->nfat_arch)
	{
		ft_printf("architecture ");
		ft_printf("%ld\n", i);
		ft_printf("    cputype %d\n", arc[i].cputype);
		ft_printf("    cpusubtype %d\n", arc[i].cpusubtype);
		ft_printf("    offset %d", arc[i].offset);
		if (arc[i].offset > size)
			ft_printf(" (past end of file)");
		if (arc[i].offset % (1 << arc[i].align) != 0)
			ft_printf(" (not aligned on it's"
					"alignment (2^%lu))\n", arc[i].align);
		else
			ft_printf("\n");
		ft_printf("    size %u", arc[i].size);
		(arc[i].offset + arc[i].size > size) ? ft_printf(" (past"
				"end of file)\n") : ft_printf("\n");
		ft_printf("    align 2^%lu (%d)\n", arc[i].align, 1 << arc[i].align);
	}
}
