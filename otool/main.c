/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 17:30:23 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/13 00:22:17 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_otool.h"

int		main(int ac, char **av)
{
	t_prg		prg;
	t_flags		f;
	int			err;
	size_t		i;

	ft_memset(&prg, 0, sizeof(prg));
	i = parse_flag(&f, av, ac, 1);
	prg.pnam = av[0];
	prg.target = av[i] ? av[i] : NULL;
	prg.mul = av[i] && av[i + 1] ? 1 : 0;
	if (!prg.target)
	{
		ft_dprintf(2, "otool <opts> <object file>\n");
		return (1);
	}
	prg.proc = &processor;
	while (prg.target)
	{
		if (prg.mul)
			ft_printf("%s:\n", prg.target);
		err = ofile_create(&prg, &f);
		if (err)
			err = ft_error(prg.target, err);
		i++;
		prg.target = av[i];
	}
	return (err);
}
