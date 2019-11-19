/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   member_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 15:22:17 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/14 14:22:02 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ofile.h"

void	ofile_member_debug(t_ofile *ofile)
{
	//ft_printf("ofile name : [%s]\n", of->member_name);
	printf("mh && mh64 :%p - %p\n", (void *)ofile->mh, (void *)ofile->mh64);
	printf("file_name = %s\n", ofile->file_name);
	printf("file_addr = 0x%x\n", (unsigned int)ofile->file_addr);
	printf("file_size = 0x%x\n", (unsigned int)ofile->file_size);
	printf("file_type = 0x%x\n", (unsigned int)ofile->file_type);
	printf("fat_header = 0x%x\n", (unsigned int)ofile->fat_header);
	printf("fat_archs = 0x%x\n", (unsigned int)ofile->fat_archs);
	printf("narch = 0x%x\n", (unsigned int)ofile->narch);
	printf("arch_type = 0x%x\n", (unsigned int)ofile->arch_type);
	printf("arch_flag.name = %s\n", ofile->arch_flag.name);
	printf("arch_flag.cputype = 0x%x\n",
		(unsigned int)ofile->arch_flag.cputype);
	printf("arch_flag.cpusubtype = 0x%x\n",
		(unsigned int)ofile->arch_flag.cpusubtype);
	printf("member_offset = 0x%x\n", (unsigned int)ofile->member_offset);
	printf("member_name = %s\n", ofile->member_name);
	printf("member_addr = 0x%x\n", (unsigned int)ofile->member_addr);
	printf("member_size = 0x%x\n", (unsigned int)ofile->member_size);
	printf("member_ar_hdr = 0x%x\n", (unsigned int)ofile->member_ar_hdr);
	printf("member_type = 0x%x\n", (unsigned int)ofile->member_type);
	printf("object_addr = 0x%x\n", (unsigned int)ofile->object_addr);
	printf("object_size = 0x%x\n", (unsigned int)ofile->object_size);
	printf("object_byte_sex = 0x%x\n",
		(unsigned int)ofile->object_byte_sex);
	printf("mh = 0x%x\n", (unsigned int)ofile->mh);
	printf("load_commands = 0x%x\n", (unsigned int)ofile->load_commands);
}
