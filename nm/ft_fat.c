/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 01:58:23 by dbaffier          #+#    #+#             */
/*   Updated: 2019/10/26 21:31:24 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <mach-o/fat.h>

/*uint32_t	fat(void *file)
{
	size_t				iter;
	struct fat_header	*hdr;
	struct fat_arch		*arch;

	hdr = (struct fat_header *)file;
	arch = (struct fat_arch)(file + sizeof(struct fat_header));
	iter = hdr.magic == FAT_MAGIC ? hdr.nfat_arch : swap_bytes(hdr.nfat_arc);
	
	return (0);
}*/
