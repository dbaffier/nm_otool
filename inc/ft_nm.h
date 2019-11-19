/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/14 13:52:31 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/19 17:35:28 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

#include "ft_ofile.h"

typedef struct		s_flags
{
	int				a;
}					t_flags;

struct symbol {
	char					*name;
	char					*indr_name;
	struct nlist_64			nl;
};

typedef struct		s_process_flg
{
	uint32_t					nsects;
	unsigned char				text_nsect;
	unsigned char				data_nsect;
	unsigned char				bss_nsect;
	struct section				**sections;
	struct section_64			**sections64;
}					t_process_flg;

typedef struct		s_nm
{
	uint32_t					ncmds;
	uint32_t					mh_flags;
	uint32_t					nsymbs;
	struct symtab_command		*st;
	struct symbol				*select_sym;
	struct symbol				*sym;;
	struct nlist				*nl;
	struct nlist_64				*nl64;
	struct s_process_flg		*f;
}					t_nm;

void	make_symbol_64(struct symbol *symbol, struct nlist_64 *nl);
void	make_symbol_32(struct symbol *symbol, struct nlist *nl);
int		select_symbol(struct symbol *symbol, char *flg, char *proc_flg);
void	select_print_symbols(t_nm *nm, char *str, int size);
int		type_symbol(t_process_flg *f, struct symbol sl);
void	print_header(t_ofile *of, char *cmd);
void	print_symbols(t_ofile *of, t_nm *nm);

#endif
