/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mach_o.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 18:50:16 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/01 01:28:17 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ofile.h"


static int		ft_mach_o_64(t_ofile *ofile, void *addr, uint32_t magic, enum byte_sex e)
{
	ofile->file_type = OFILE_Mach_O;
	ofile->object_addr = addr;
	ofile->object_size = ofile->file_size;
	if (magic == MH_MAGIC_64)
		ofile->object_byte_sex = e;
	else
		ofile->object_byte_sex = (e == BIG_ENDIAN_BYTE_SEX)
			? LITTLE_ENDIAN_BYTE_SEX : BIG_ENDIAN_BYTE_SEX;
	ofile->mh64 = (struct mach_header_64 *)addr;
	ofile->load_commands = (struct load_command *)(addr + sizeof(struct mach_header_64));
	return (0);
}

int		ft_mach_o(t_ofile *ofile, void *addr, uint32_t magic, enum byte_sex e)
{
	if (ofile->file_size >= sizeof(struct mach_header) &&
			(magic == MH_MAGIC || magic == swap_uint32(MH_MAGIC)))
	{
		ofile->file_type = OFILE_Mach_O;
		ofile->object_addr = addr;
		ofile->object_size = ofile->file_size;
		if (magic == MH_MAGIC)
			ofile->object_byte_sex = e;
		else
			ofile->object_byte_sex = (e == BIG_ENDIAN_BYTE_SEX)
				? LITTLE_ENDIAN_BYTE_SEX : BIG_ENDIAN_BYTE_SEX;
		ofile->mh = (struct mach_header *)addr;
		ofile->load_commands = (struct load_command *)(addr + sizeof(struct mach_header));
	}
	else if (ofile->file_size >= sizeof(struct mach_header_64) &&
			(magic == MH_MAGIC_64 || magic == swap_uint32(MH_MAGIC_64)))
		ft_mach_o_64(ofile, addr, magic, e);
	return (0);
}
