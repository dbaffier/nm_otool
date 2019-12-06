/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 23:35:23 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/06 21:58:23 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <mach-o/stab.h>

void	print_header(t_ofile *of, char *cmd)
{
	(void)cmd;
	if (of->member_ar_hdr)
		ft_printf("\n%s(%.*s):\n", of->file_name, (int)of->member_name_size, of->member_name);
}

static void	quicksort(struct symbol *symbol, int size)
{
	struct symbol tmp;
	int		i;

	i = 0;
	while (i < size)
	{
		if (i + 1 < size && ft_strcmp(symbol[i + 1].name, symbol[i].name) < 0)
		{
			tmp = symbol[i + 1];
			symbol[i + 1] = symbol[i];
			symbol[i] = tmp;
			i = 0;
		}
		else
			i++;
	}
}

static const struct stabnames stabnames[] = {
	{ N_GSYM,  "GSYM" },
	{ N_FNAME, "FNAME" },
	{ N_FUN,   "FUN" },
	{ N_STSYM, "STSYM" },
	{ N_LCSYM, "LCSYM" },
	{ N_BNSYM, "BNSYM" },
	{ N_OPT,   "OPT" },
	{ N_RSYM,  "RSYM" },
	{ N_SLINE, "SLINE" },
	{ N_ENSYM, "ENSYM" },
	{ N_SSYM,  "SSYM" },
	{ N_SO,    "SO" },
	{ N_OSO,   "OSO" },
	{ N_LSYM,  "LSYM" },
	{ N_BINCL, "BINCL" },
	{ N_SOL,   "SOL" },
	{ N_PARAMS,"PARAM" },
	{ N_VERSION,"VERS" },
	{ N_OLEVEL,"OLEV" },
	{ N_PSYM,  "PSYM" },
	{ N_EINCL, "EINCL" },
	{ N_ENTRY, "ENTRY" },
	{ N_LBRAC, "LBRAC" },
	{ N_EXCL,  "EXCL" },
	{ N_RBRAC, "RBRAC" },
	{ N_BCOMM, "BCOMM" },
	{ N_ECOMM, "ECOMM" },
	{ N_ECOML, "ECOML" },
	{ N_LENG,  "LENG" },
	{ N_PC,    "PC" },
	{ 0, 0 }};

static char	*stab(unsigned char n_type)
{
	const struct stabnames *p;

	p = stabnames;
	while (p->name)
	{
		if (p->n_type == n_type)
			return (p->name);
		p++;
	}
	return (NULL);
}

void	print_symbols(t_ofile *of, t_nm *nm, t_flags *f)
{
	char		c;
	size_t		i;

	i = 0;
	quicksort(nm->select_sym, nm->nsymbs);
	(void)of;
	while (i < nm->nsymbs)
	{
		if (nm->select_sym[i].nl.n_type & N_STAB)
		{
			ft_printf("%016llx ", nm->select_sym[i].nl.n_value);
			ft_printf(" - %02x %04x %5.5s",
					(unsigned int)nm->select_sym[i].nl.n_sect & 0xff,
					(unsigned int)nm->select_sym[i].nl.n_desc & 0xffff,
					stab(nm->select_sym[i].nl.n_type));
			ft_printf("%s\n", nm->select_sym[i].name);
		}
		if (f->u == 0 && f->j == 0)
		{
			printf("Enter\n");
			c = type_symbol(nm->f, nm->select_sym[i]);
			if (c == 'u' || c == 'U' || c == 'i' || c == 'I')
				ft_printf("%*s ", 16, "");
			else
				ft_printf("%016llx ", nm->select_sym[i].nl.n_value);
			ft_printf("%c ", c);
		}
		ft_printf("%s\n", nm->select_sym[i].name);
		i++;
	}
}
