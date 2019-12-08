/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 21:44:32 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/08 23:20:19 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OTOOL_H
# define FT_OTOOL_H

#include "ft_ofile.h"
#include "libft.h"

typedef struct		s_flags
{
	unsigned int	f;
	unsigned int	l;
	unsigned int	h;
	unsigned int	ll;
	unsigned int	dd;
	unsigned int	t;
	unsigned int	d;
	unsigned int	ss;
	int				object_process;
}					t_flags;


typedef struct		s_data
{
	char			*addr;
	unsigned long	size;
	cpu_type_t		mh_cputype;
	cpu_subtype_t	mh_cpusubtype;
	uint32_t		mh_magic;
	uint32_t		mh_filetype;
	uint32_t		mh_ncmds;
	uint32_t		mh_sizeofcmds;
	uint32_t		sizeof_mach_header;
}					t_data;

int					parse_flag(t_flags *f, char **av, int ac, int i);
void				processor(t_ofile *of, char *arch_name, void *cookie);

void				print_header(t_ofile *of);
void				print_mach_header_64(struct mach_header_64 *mh64);
void				print_mach_header(struct mach_header *mh);
void				print_loadcmds(t_ofile *of, t_data *data);

#endif
