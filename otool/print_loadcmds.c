/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_loadcmds.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 23:09:44 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/09 21:23:13 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

void	print_loadcmds(t_ofile *of, t_data *data)
{
	struct load_command		*lc;
	struct load_command		l;
	uint32_t				i;
	int						swapped;

	swapped = get_host_byte_sex() != of->object_byte_sex;
	lc = of->load_commands;
	i = -1;
	while (++i < data->mh_ncmds)
	{
		ft_printf("Load command %u\n", i);
		ft_memcpy((char *)&l, (char *)lc, sizeof(struct load_command));
		if (l.cmd == LC_SEGMENT)
			print_lc_segment32(of, data, lc);
		else if (l.cmd == LC_SEGMENT_64)
			print_lc_segment64(of, data, lc);
		else if (l.cmd == LC_SYMTAB)
			print_lc_symtab(of, data, lc);

		lc = (struct load_command *)((char *)lc + l.cmdsize);
	}

}

/*
	void
print_loadcmds(
		struct load_command *load_commands,
		uint32_t ncmds,
		uint32_t sizeofcmds,
		cpu_type_t cputype,
		uint32_t filetype,
		enum byte_sex load_commands_byte_sex,
		uint64_t object_size,
		enum bool verbose,
		enum bool very_verbose)
{
	enum byte_sex host_byte_sex;
	enum bool swapped;
	uint32_t i, j, k, left, size, *unknown, nsyms;
	char *p, *begin, *end;
	struct load_command *lc, l;
	struct segment_command sg;
	struct section s;
	struct segment_command_64 sg64;
	struct section_64 s64;
	struct symtab_command st;
	struct dysymtab_command dyst;
	uint64_t big_load_end;

	host_byte_sex = get_host_byte_sex();
	swapped = host_byte_sex != load_commands_byte_sex;

	nsyms = UINT_MAX;
	lc = load_commands;
	big_load_end = 0;
	for(i = 0 ; i < ncmds; i++){
		printf("Load command %u\n", i);

		memcpy((char *)&l, (char *)lc, sizeof(struct load_command));
		if(swapped)
			swap_load_command(&l, host_byte_sex);
		switch(l.cmd){
			case LC_SEGMENT:
				memset((char *)&sg, '\0', sizeof(struct segment_command));
				size = left < sizeof(struct segment_command) ?
					left : sizeof(struct segment_command);
				memcpy((char *)&sg, (char *)lc, size);
				if(swapped)
					swap_segment_command(&sg, host_byte_sex);
				print_segment_command(sg.cmd, sg.cmdsize, sg.segname,
						sg.vmaddr, sg.vmsize, sg.fileoff, sg.filesize,
						sg.maxprot, sg.initprot, sg.nsects, sg.flags,
						object_size, verbose);
				p = (char *)lc + sizeof(struct segment_command);
				for(j = 0 ; j < sg.nsects ; j++){
					left = sizeofcmds - (p - (char *)load_commands);
					memset((char *)&s, '\0', sizeof(struct section));
					size = left < sizeof(struct section) ?
						left : sizeof(struct section);
					memcpy((char *)&s, p, size);
					if(swapped)
						swap_section(&s, 1, host_byte_sex);
					print_section(s.sectname, s.segname, s.addr, s.size,
							s.offset, s.align, s.reloff, s.nreloc, s.flags,
							s.reserved1, s.reserved2, sg.cmd, sg.segname,
							filetype, object_size, verbose);
					if(p + sizeof(struct section) >
							(char *)load_commands + sizeofcmds)
						return;
					p += size;
				}
				break;

			case LC_SEGMENT_64:
				memset((char *)&sg64, '\0', sizeof(struct segment_command_64));
				size = left < sizeof(struct segment_command_64) ?
					left : sizeof(struct segment_command_64);
				memcpy((char *)&sg64, (char *)lc, size);
				if(swapped)
					swap_segment_command_64(&sg64, host_byte_sex);
				print_segment_command(sg64.cmd, sg64.cmdsize, sg64.segname,
						sg64.vmaddr, sg64.vmsize, sg64.fileoff, sg64.filesize,
						sg64.maxprot, sg64.initprot, sg64.nsects, sg64.flags,
						object_size, verbose);
				p = (char *)lc + sizeof(struct segment_command_64);
				for(j = 0 ; j < sg64.nsects ; j++){
					if(p + sizeof(struct section_64) >
							(char *)load_commands + sizeofcmds){
						printf("section structure command extends past end of "
								"load commands\n");
					}
					left = sizeofcmds - (p - (char *)load_commands);
					memset((char *)&s64, '\0', sizeof(struct section_64));
					size = left < sizeof(struct section_64) ?
						left : sizeof(struct section_64);
					memcpy((char *)&s64, p, size);
					if(swapped)
						swap_section_64(&s64, 1, host_byte_sex);
					print_section(s64.sectname, s64.segname, s64.addr,
							s64.size, s64.offset, s64.align, s64.reloff,
							s64.nreloc, s64.flags, s64.reserved1, s64.reserved2,
							sg64.cmd, sg64.segname, filetype, object_size,
							verbose);
					if(p + sizeof(struct section_64) >
							(char *)load_commands + sizeofcmds)
						return;
					p += size;
				}
				break;

			case LC_SYMTAB:
				memset((char *)&st, '\0', sizeof(struct symtab_command));
				size = left < sizeof(struct symtab_command) ?
					left : sizeof(struct symtab_command);
				memcpy((char *)&st, (char *)lc, size);
				if(swapped)
					swap_symtab_command(&st, host_byte_sex);
				nsyms = st.nsyms;
				print_symtab_command(&st, cputype, object_size);
				break;
		}
		if(l.cmdsize == 0){
			printf("load command %u size zero (can't advance to other "
					"load commands)\n", i);
			return;
		}
		lc = (struct load_command *)((char *)lc + l.cmdsize);
		if((char *)lc > (char *)load_commands + sizeofcmds)
			return;
	}
}*/
