/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/14 13:52:31 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/08 21:07:48 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <stdbool.h>
# include "ft_ofile.h"

# define ARGS0 "	-a\tDisplay all symbol table entries.\n"
# define ARGS1 "	-g\tDisplay only global (external) symbols.\n"
# define ARGS3 "	-o\tPrepend file or archive element name to each line.\n"
# define ARGS4 "	-p\tDon't sort; display in symbol-table order.\n"
# define ARGS6 "	-u\tDisplay only undefined symbols.\n"
# define ARGS7 "	-U\tDon't display undefined symbols.\n"
# define ARGS8 "	-j\tJust display the symbol names (no value or type).\n"
# define ARGS9 "	-A\tWrite the pathname or library name on each line.\n"

typedef struct		s_flags
{
	unsigned int			a;
	unsigned int			g;
	unsigned int			o;
	unsigned int			p;
	unsigned int			u;
	unsigned int			uu;
	unsigned int			j;
	unsigned int			aa;
}					t_flags;

struct				s_symbol
{
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

struct				s_stabnames
{
	unsigned char				n_type;
	char						*name;
};

typedef struct		s_nm
{
	uint32_t					ncmds;
	uint32_t					mh_flags;
	uint32_t					nsymbs;
	struct symtab_command		*st;
	struct s_symbol				*select_sym;
	struct s_symbol				*sym;
	struct nlist				*nl;
	struct nlist_64				*nl64;
	t_process_flg				*flg;
}					t_nm;

int					print_stab(t_nm *nm, size_t i);
int					parse_flag(t_flags *f, char **av, int ac, int i);
void				make_symbol_64(struct s_symbol *symbol,
		struct nlist_64 *nl);
void				make_symbol_32(struct s_symbol *symbol, struct nlist *nl);
int					select_symbol(struct s_symbol *symbol, t_flags *f);
void				select_print_symbols(t_nm *nm, t_ofile *of, int size);
int					type_symbol(t_process_flg *f, struct s_symbol sl);
void				print_header(t_ofile *of, t_flags *f);
void				print_symbols(t_ofile *of, t_nm *nm, t_flags *f);
void				process_flg_sect(t_nm *nm, t_ofile *of,
		t_process_flg *f, struct load_command *lc);

#endif
