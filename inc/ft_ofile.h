/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prg.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 01:03:24 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/16 22:48:09 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Ft_prg_H
# define Ft_prg_H

#include "libft.h"
#include "ft_printf.h"
#include <mach-o/arch.h>
#include <mach-o/fat.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <ar.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>


enum {
	ERR_MALLOC = 1,
	ERR_OPEN,
	ERR_STAT,
	ERR_MAP,
	ERR_WRITE,
	ERR_ARCHIVE,
	ERR_ARCHIVE_EMPTY,
};

enum ofile_type {
	OFILE_UNKNOWN,
	OFILE_FAT,
	OFILE_ARCHIVE,
	OFILE_Mach_O
};

enum byte_sex {
    UNKNOWN_BYTE_SEX,
    BIG_ENDIAN_BYTE_SEX,
    LITTLE_ENDIAN_BYTE_SEX
};

typedef struct	s_arch_flag
{
	char				*name;
	cpu_type_t			cputype;
	cpu_subtype_t		cpusubtype;
}				t_arch_flag;

typedef struct		s_ofile
{
	char				*file_name;		    /* pointer to name malloc'ed by ofile_map */
	char				*file_addr;		    /* pointer to vm_allocate'ed memory       */
	uint32_t			file_size;	    	/* size of vm_allocate'ed memory	      */
	uint64_t			file_mtime;			/* stat(2)'s mtime                        */
	enum ofile_type		file_type;			/* type of the file			      */

	struct fat_header	*fat_header;		/* If a fat file these are filled in and */
	struct fat_arch		*fat_archs;			/*  if needed converted to host byte sex */

	/* If this is a fat file then these are valid and filled in */
	uint32_t			narch;	    	    /* the current architecture */
	enum ofile_type		arch_type;			/* the type of file for this arch. */
	t_arch_flag			arch_flag;
	/*  field is pointing at space malloc'ed */
	/*  by ofile_map. */

	/* If this structure is currently referencing an archive member or an object
	   file that is an archive member these are valid and filled in. */
	uint32_t			member_offset;      /* logical offset to the member starting */
	char				*member_addr;		/* pointer to the member contents */
	uint32_t			member_size;		/* actual size of the member (not rounded)*/
	struct ar_hdr		*member_ar_hdr;		/* pointer to the ar_hdr for this member */
	char				*member_name;		/* name of this member */
	uint32_t			member_name_size;   /* size of the member name */
	enum ofile_type		member_type;		/* the type of file for this member */
	//cpu_type_t			archive_cputype;	/* if the archive contains objects then */
	//cpu_subtype_t		archive_cpusubtype;	    /*  at are in the archive. */

	/* If this structure is currently referencing a dynamic library module 
	   these are valid and filled in. */
	//struct dylib_module	*modtab;    /* the 32-bit module table */
	//struct dylib_module_64 *modtab64;/* the 64-bit module table */
	uint32_t nmodtab;	    	    /* the number of module table entries */
	//struct dylib_module		    /* pointer to the 32-bit dylib_module for */
	//	*dylib_module;		    /*  this module. */
	//struct dylib_module_64	    /* pointer to the 64-bit dylib_module for */
		//*dylib_module64;	    /*  this module. */
	char *dylib_module_name;	    /* the name of the module */

	/* If this structure is currently referencing an object file these are
	   valid and filled in.  The mach_header and load commands have been 
	   converted to the host byte sex if needed */
	/*enum bool headers_swapped;	*/    /* true if the headers have already been
									   swapped to host byte sex */
	char						*object_addr;		    /* the address of the object file */
	uint32_t					object_size;	    /* the size of the object file */
	enum byte_sex				object_byte_sex;  /* the byte sex of the object file */
	struct mach_header			*mh;	    /* the mach_header of 32-bit object file */
	struct mach_header_64		*mh64;    /* the mach_header of 64-bit object file */
	struct load_command		  	*load_commands;
	/* these copied from the mach header above */
	//cpu_type_t mh_cputype;	    /* cpu specifier */
	//cpu_subtype_t mh_cpusubtype;    /* machine specifier */
	uint32_t mh_filetype;	    /* type of file */
}					t_ofile;

typedef struct		s_prg
{
	char			*target;
	t_ofile			*file;
	char			*pnam;
	void			(*proc)(t_ofile *of, char *arch_name, void *cookie);
}					t_prg;


int					ft_fat(t_ofile *of, void *addr, uint32_t magic, enum byte_sex e);
int					ft_archive(t_ofile *of, void *addr, uint32_t magic, enum byte_sex e);
int					ft_mach_o(t_ofile *of, void *addr, uint32_t magic, enum byte_sex e);

int					ofile_first_member(t_ofile *of);
int					ofile_next_member(t_ofile *ofile);
void				ofile_unmap(t_prg *nm_t, t_ofile *of);

void				ofile_member_clear(t_ofile *of);
void				ofile_member_debug(t_ofile *of);
void				ofile_object_clear(t_ofile *of);

unsigned long		size_ar_name(struct ar_hdr *ar_hdr);
unsigned long		ft_strtoul(const char *ptr, char **endptr, int base);

int					process_archive(t_prg *nm_t, t_ofile *of);
void				nm(t_ofile *ofile, char *arch_name, void *cookie);


uint32_t			process_fat(t_prg *nm_t, t_ofile *of);

//uint32_t	fat(void *file);
int			ofile_create(t_prg *nm_t);
int			nm_routine(t_prg *nm_t, void *addr);
int			nm_err(t_prg *nm, int err);

uint16_t	swap_uint16(uint16_t val);
uint32_t	swap_uint32(uint32_t val);
int16_t		swap_int16(int16_t val);
int32_t		swap_int32(int32_t val);
long long	swap_long_long(long long ll);
short		swap_short(short s);

int				ft_fat(t_ofile *of, void *addr, uint32_t magic, enum byte_sex e);
enum byte_sex	get_host_byte_sex(void);
void			swap_nlist(struct nlist *symbols, uint32_t nsymbols);
void			swap_nlist_64(struct nlist_64 *symbols, uint32_t nsymbols);
void			swap_fat_arch(struct fat_arch *arch, unsigned long n_fat_arch);
void			swap_fat_header(struct fat_header *hdr);
void			print_fat_headers(struct fat_header *hdr, struct fat_arch *arc, unsigned long size);
int				ofile_get_arch(t_ofile *of, struct fat_header *hdr, struct fat_arch *arch, int i);

#endif
