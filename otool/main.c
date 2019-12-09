/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 17:30:23 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/09 20:36:23 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_otool.h"

int main(int ac, char **av)
{
	t_prg		prg;
	t_flags		f;
	int			err;
	size_t		i;

	ft_memset(&prg, 0, sizeof(prg));
	i = parse_flag(&f, av, ac, 1);
	prg.pnam = av[0];
	prg.target = av[i] ? av[i] : "a.out";
	prg.proc = &processor;
	while (prg.target)
	{
		err = ofile_create(&prg, &f);
		i++;
		prg.target = av[i];
	}
	return (err);
}
