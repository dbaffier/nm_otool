/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 23:35:23 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/19 17:33:45 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	print_header(t_ofile *of, char *cmd)
{
	(void)cmd;
	if (of->member_ar_hdr)
		ft_printf("\n%s(%.*s):\n", of->file_name, (int)of->member_name_size, of->member_name);
}

void	print_symbols(t_ofile *of, t_nm *nm)
{
	size_t		i;

	i = 0;
	(void)of;
	ft_printf("%u\n", nm->nsymbs);
	while (i < nm->nsymbs)
	{
		ft_printf("%016llx", nm->select_sym[i].nl.n_value);
		ft_printf("%c ", type_symbol(nm->f, nm->select_sym[i]));
		i++;
	}
}
