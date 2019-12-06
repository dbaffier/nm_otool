/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_arch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 03:47:28 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/06 21:54:48 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int	ofile_flood_mach_o(t_ofile *of)
{
	enum byte_sex	b;

	if (size >= sizeof(struct mach_header) &&
			(magic == MH_MAGIC || magic = swap_uint32(MH_MAGIC)))
	{
		ofile->arch_type = OFILE_Mach_O;
		ofile->object_addr = addr;
		ofile->object_size = size;
		b = get_host_byte_sex();
		if (magic == MH_MAGIC)
			ofile->object_byte_sex = b;
		else
			ofile->object_byte_sex = (b == BIG_ENDIAN_BYTE_SEX)
					? LITTLE_ENDIAN_BYTE_SEX : BIG_ENDIAN_BYTE_SEX;
		ofile->mh = (struct mach_header *)addr;
		ofile->load_commands = (struct load_command *)(addr + sizeof(struct mach_header));
	}
}

static int	ofile_get_arch(t_ofile *of, unsigned long narch)
{
	char			*addr;
	unsigned long	size;
	unsigned long	magic;

	ofile->narch = narch;
	ofile->arch_type = OFILE_UNKNOWN;
	ofile->object_addr = NULL;
	ofile->objet_size = 0;
	ofile->object_byte_sex = UNKNOWN_BYTE_SEX;
	ofile->mh = NULL;
	ofile->load_commands = NULL;
	if (ofile->file_type == OFILE_FAT)
	{
		ofile->member_offset = 0;
		ofile->member_addr = NULL;
		ofile->member_size = 0;
		ofile->member_ar_hdr = NULL;
		ofile->member_type = OFILE_UNKNOWN;
		size = ofile->fat_archs[ofile->narch].size;
		addr = ofile->file_addr + ofile->fat_archs[ofile->narch].offset;
	}
	if (size >= sizeof(struct mach_header))
		memcpy(&magic, addr, sizeof(unsigned long));
	ofile_flood_mach_o(of);
}

int		ofile_first_arch(t_ofile *of)
{
	if (of->file_type == OFILE_FAT
			|| of->file_type == OFILE_ARCHIVE
			&& of->file_member_type == OFILE_FAT)
		return (ofile_get_arch(of, 0));
	else
		// error
		;
	return (0);
}
