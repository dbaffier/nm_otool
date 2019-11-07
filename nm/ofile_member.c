/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ofile_member.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 23:45:51 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/07 16:39:06 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int		ofile_ext_format(t_ofile *ofile, void *addr, unsigned long offset)
{
	struct ar_hdr	*ar_hdr;
    unsigned long	ar_name_size;

	ar_hdr = (struct ar_hdr *)(addr + offset);
	offset += sizeof(struct ar_hdr);
	ofile->member_offset = offset;
	ofile->member_addr = addr + offset;
	ofile->member_size = ft_strtoul(ar_hdr->ar_size, NULL, 10);
	ofile->member_ar_hdr = ar_hdr;
	if (ft_strncmp(ofile->member_name, AR_EFMT1, sizeof(AR_EFMT1) - 1) == 0)
	{
		ofile->member_name = ar_hdr->ar_name + sizeof(struct ar_hdr);
		ar_name_size = ft_strtoul(ar_hdr->ar_name + sizeof(AR_EFMT1) - 1, NULL, 10);
		ofile->member_name_size = ar_name_size;
		ofile->member_offset += ar_name_size;
		ofile->member_addr += ar_name_size;
		ofile->member_size -= ar_name_size;
	}
	else
	{
		ofile->member_name_size = size_ar_name(ar_hdr);
		ofile->member_name_size = 0;
		ar_name_size = 0;
	}
	return (0);
}

static int		ofile_set_fat(t_ofile *ofile)
{
	uint32_t		magic;

	ft_memcpy(&magic, ofile->member_addr, sizeof(unsigned long));
	if (magic == FAT_MAGIC || magic == swap_uint32(FAT_MAGIC))
	{
		ofile->member_type = OFILE_FAT;
		ofile->fat_header = (struct fat_header *)(ofile->member_addr);
		if (magic == swap_uint32(FAT_MAGIC))
			swap_fat_header(ofile->fat_header);
		ofile->fat_archs = (struct fat_arch *)(ofile->member_addr + sizeof(struct fat_header));
	}
	return (0);
}

static int		ofile_set_mach_o(t_ofile *of, unsigned long offset, unsigned long size)
{
	uint32_t		magic;
	enum byte_sex	e;

	ft_memcpy(&magic, of->member_addr, sizeof(unsigned long));
	e = get_host_byte_sex();
	if (size - (offset + of->member_name_size) >= sizeof(struct mach_header)
			&& (magic == MH_MAGIC || magic == swap_uint32(FAT_MAGIC)))
	{
		of->member_type = OFILE_Mach_O;
		of->object_addr = of->member_addr;
		of->object_size = of->member_size;
		if (magic == MH_MAGIC)
			of->object_byte_sex = e;
		else
			of->object_byte_sex = (e == BIG_ENDIAN_BYTE_SEX) ? LITTLE_ENDIAN_BYTE_SEX:
				BIG_ENDIAN_BYTE_SEX;
		of->mh = (struct mach_header *)of->object_addr;
		of->load_commands = (struct load_command *)(of->object_addr + sizeof(struct mach_header));
	}
	return (0);
}

int		ofile_next_member(t_ofile *ofile)
{
	void			*addr;
	unsigned long	size;
	unsigned long	offset;

	ofile_member_clear(ofile);
	if (ofile->file_type == OFILE_ARCHIVE)
	{
		addr = ofile->file_addr;
		size = ofile->file_size;
	}
	else
		return (1);
	offset = ofile->member_offset + ft_round(ofile->member_size, sizeof(short));
	if (offset == size)
		return (1);
	offset += sizeof(struct ar_hdr);
	ofile_ext_format(ofile, addr, offset);
	ofile_object_clear(ofile);
	if (ofile->member_size > sizeof(unsigned long))
	{
		ofile_set_fat(ofile);
		ofile_set_mach_o(ofile, offset, size);
	}
	return (0);
}

int		ofile_first_member(t_ofile *of)
{
	void			*addr;
	unsigned long	size;
	unsigned long	offset;
	unsigned long	magic;
	enum byte_sex	e;
    struct ar_hdr	*ar_hdr;
    unsigned long	ar_name_size;

	of->member_offset = 0;
	of->member_addr = NULL;
	of->member_size = 0;
	of->member_ar_hdr = NULL;
	of->member_name = NULL;
	of->member_name_size = 0;
	of->member_type = OFILE_UNKNOWN;
	of->object_addr = NULL;
	of->object_size = 0;
	of->object_byte_sex = UNKNOWN_BYTE_SEX;
	of->mh = NULL;
	of->load_commands = NULL;

	printf("%p\n", of->file_addr);
	if (of->file_type == OFILE_ARCHIVE)
	{
		addr = of->file_addr;
	    size = of->file_size;
	}
	else
		return (1);
	printf("Hey\n");
	if(size < SARMAG || strncmp(addr, ARMAG, SARMAG) != 0)
		return (1);
	offset = SARMAG;
	if(offset != size && offset + sizeof(struct ar_hdr) > size)
	    return(ERR_ARCHIVE);
	if(size == offset)
	    return(ERR_ARCHIVE_EMPTY);
	ar_hdr = (struct ar_hdr *)(addr + offset);
	offset += sizeof(struct ar_hdr);
	of->member_offset = offset;
	of->member_addr = addr + offset;
	of->member_size = ft_strtoul(ar_hdr->ar_size, NULL, 10);
	of->member_ar_hdr = ar_hdr;
	of->member_type = OFILE_UNKNOWN;
	of->member_name = ar_hdr->ar_name;
	if (ft_strncmp(of->member_name, AR_EFMT1, sizeof(AR_EFMT1) - 1) == 0)
	{
	    of->member_name = ar_hdr->ar_name + sizeof(struct ar_hdr);
	    ar_name_size = ft_strtoul(ar_hdr->ar_name + sizeof(AR_EFMT1) - 1,
				   NULL, 10);
		printf("---%ld\n", ar_name_size);
	//	printf("ar size - %lu\n", ar_name_size);
	    of->member_name_size = ar_name_size;
	//printf("--%u\n", of->member_offset);
	    of->member_offset += ar_name_size;
	    of->member_addr += ar_name_size;
	    of->member_size -= ar_name_size;
	}
	else
	{
	    of->member_name_size = size_ar_name(ar_hdr);
	    ar_name_size = 0;
	}
	e = get_host_byte_sex();
	if (of->member_size > sizeof(unsigned long))
	{
		ft_memcpy(&magic, of->member_addr, sizeof(unsigned long));
		if (magic == FAT_MAGIC || magic == swap_uint32(FAT_MAGIC))
		{
			of->member_type = OFILE_FAT;
			of->fat_header = (struct fat_header *)(of->member_addr);
			if (magic == swap_uint32(FAT_MAGIC))
				swap_fat_header(of->fat_header);
			of->fat_archs = (struct fat_arch *)(of->member_addr + sizeof(struct fat_header));
				
		}
		if (size - (offset + ar_name_size) >= sizeof(struct mach_header) &&
				(magic == MH_MAGIC || magic == swap_uint32(FAT_MAGIC)))
		{
			of->member_type = OFILE_Mach_O;
			of->object_addr = of->member_addr;
			of->object_size = of->member_size;
			if (magic == MH_MAGIC)
				of->object_byte_sex = e;
			else
				of->object_byte_sex = (e == BIG_ENDIAN_BYTE_SEX) ? LITTLE_ENDIAN_BYTE_SEX : BIG_ENDIAN_BYTE_SEX;
			of->mh = (struct mach_header *)(of->object_addr);
			of->load_commands = (struct load_command *)(of->object_addr + sizeof(struct mach_header));
		}
	}
	return (0);
}
