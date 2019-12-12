/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ofile.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/07 20:29:47 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/12 20:17:42 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OFILE_H
# define FT_OFILE_H

# include "libft.h"
# include "ft_printf.h"
# include <mach-o/arch.h>
# include <mach-o/fat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <ar.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <sys/stat.h>

enum {
	ERR_MALLOC = 1,
	ERR_OPEN,
	ERR_STAT,
	ERR_MAP,
	ERR_WRITE,
	ERR_ARCHIVE,
	ERR_ARCHIVE_EMPTY,
};

enum				e_ofile_type
{
	OFILE_UNKNOWN,
	OFILE_FAT,
	OFILE_ARCHIVE,
	OFILE_Mach_O
};

enum				e_byte_sex
{
	UNKNOWN_BYTE_SEX,
	BIG_ENDIAN_BYTE_SEX,
	LITTLE_ENDIAN_BYTE_SEX
};

typedef struct		s_ofile
{
	short						prog;
	char						*file_name;
	char						*file_addr;
	uint32_t					file_size;
	uint64_t					file_mtime;
	enum e_ofile_type			file_type;
	struct fat_header			*fat_header;
	struct fat_arch				*fat_archs;
	uint32_t					member_offset;
	char						*member_addr;
	uint32_t					member_size;
	struct ar_hdr				*member_ar_hdr;
	char						*member_name;
	uint32_t					member_name_size;
	enum e_ofile_type			member_type;
	char						*object_addr;
	uint32_t					object_size;
	enum e_byte_sex				object_byte_sex;
	struct mach_header			*mh;
	struct mach_header_64		*mh64;
	struct load_command			*load_commands;
}					t_ofile;

typedef struct		s_prg
{
	char			*target;
	char			*pnam;
	void			(*proc)(t_ofile *of,
			char *arch_name, void *cookie);
}					t_prg;

uint32_t			ft_fat(t_ofile *of, void *addr,
		uint32_t magic, enum e_byte_sex e);
int					ft_archive(t_ofile *of, void *addr,
		uint32_t magic, enum e_byte_sex e);
int					ft_mach_o(t_ofile *of, void *addr,
		uint32_t magic, enum e_byte_sex e);

int					ofile_first_member(t_ofile *of);
int					ofile_next_member(t_ofile *ofile);
void				ofile_unmap(t_prg *nm_t, t_ofile *of);

void				ofile_member_clear(t_ofile *of);
void				ofile_member_debug(t_ofile *of);
void				ofile_object_clear(t_ofile *of);

unsigned long		size_ar_name(struct ar_hdr *ar_hdr);
unsigned long		ft_strtoul(const char *ptr, char **endptr, int base);

int					process_archive(t_prg *nm_t, t_ofile *of, void *cookie);
void				nm(t_ofile *ofile, char *arch_name, void *cookie);

int					ofile_create(t_prg *nm_t, void *cookie);
int					nm_routine(t_prg *nm_t, void *addr);
int					nm_err(t_prg *nm, int err);

uint16_t			swap_uint16(uint16_t val);
uint32_t			swap_uint32(uint32_t val);
int16_t				swap_int16(int16_t val);
int32_t				swap_int32(int32_t val);
unsigned long		swap_long(unsigned int val);
long long			swap_long_long(long long ll);
short				swap_short(short s);

enum e_byte_sex		get_host_byte_sex(void);
void				swap_nlist(struct nlist *symbols, uint32_t nsymbols);
void				swap_nlist_64(struct nlist_64 *symbols, uint32_t nsymbols);
void				swap_fat_arch(struct fat_arch *arch,
		unsigned long n_fat_arch);
void				swap_fat_header(struct fat_header *hdr);
void				print_fat_headers(struct fat_header *hdr,
		struct fat_arch *arc, unsigned long size);
int					ofile_get_arch(t_ofile *of, struct fat_header *hdr,
		struct fat_arch *arch, int i);

#endif
