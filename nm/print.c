/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 23:35:23 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/01 03:04:27 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

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

void	print_symbols(t_ofile *of, t_nm *nm)
{
	char		c;
	size_t		i;

	i = 0;
	(void)of;
	quicksort(nm->select_sym, nm->nsymbs);
	while (i < nm->nsymbs)
	{
		c = type_symbol(nm->f, nm->select_sym[i]);
		if (c == 'u' || c == 'U' || c == 'i' || c == 'I')
			ft_printf("%*s ", 16, "");
		else
			ft_printf("%016llx ", nm->select_sym[i].nl.n_value);
		ft_printf("%c ", c);
		ft_printf("%s\n", nm->select_sym[i].name);
		i++;
	}
}
