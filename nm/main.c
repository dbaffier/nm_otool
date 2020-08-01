/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 00:35:15 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/12 23:50:38 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "ft_ofile.h"
#include "ft_nm.h"

static int ft_error(char *s, int err)
{
	static char *tab[] = {NULL, 
	"%s malloc error",
	"%s open error",
	"%s stat error",
	"%s MMAP error",
	"%s write error",
	"%s archive err",
	"%s archive empty error"};
	
	ft_printf("ft_nm: ");
	ft_printf(tab[err], s);
	ft_printf("\n");
	return (err);
}

/*
** Main entry of nm
** Initialize t_prg, parse flag in t_flags structure (see ft_nm.h)
** All options parsed, if no filename specified, do a.out
** Loop over filename if given,
** Create an ofile structure containing informations about filename
** Apply flag and execute is given routine (&nm)
*/

int		main(int ac, char **av)
{
	t_prg		prg;
	t_flags		f;
	int			err;
	size_t		i;

	ft_memset(&prg, 0, sizeof(prg));
	prg.pnam = av[0];
	i = parse_flag(&f, av, ac, 1);
	prg.target = av[i] ? av[i] : "a.out";
	prg.proc = &nm;
	while (prg.target)
	{
		err = ofile_create(&prg, &f);
		i++;
		if (err)
			return (ft_error(prg.target, err));
		prg.target = av[i];
	}
	return (err);
}
