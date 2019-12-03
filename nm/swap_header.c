/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_header.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 23:45:40 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/01 01:13:53 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ofile.h"
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

void	swap_nlist(struct nlist *symbols, uint32_t nsymbols)
{
	uint32_t		i;

	i = 0;
	while (i < nsymbols)
	{
		symbols[i].n_un.n_strx = swap_int32(symbols[i].n_un.n_strx);
		symbols[i].n_desc = swap_short(symbols[i].n_desc);
		symbols[i].n_value = swap_int32(symbols[i].n_value);
		i++;
	}

}

#define SWAP_INT(a)  ( ((a) << 24) | \
		      (((a) << 8) & 0x00ff0000) | \
		      (((a) >> 8) & 0x0000ff00) | \
	 ((unsigned int)(a) >> 24) )

void	swap_nlist_64(struct nlist_64 *symbols, uint32_t nsymbols)
{
	uint32_t		i;

	i = 0;
	while (i < nsymbols)
	{
		symbols[i].n_un.n_strx = SWAP_INT(symbols[i].n_un.n_strx);
		symbols[i].n_desc = swap_short(symbols[i].n_desc);
		symbols[i].n_value = swap_long_long(symbols[i].n_value);
		i++;
	}
}
