/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 00:35:15 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/01 18:05:20 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_nm.h"
#include "ft_printf.h"
#include "ft_ofile.h"

int main(int ac, char **av)
{
	t_prg		prg;
	t_flags		f;
	int			err;
	size_t		i;

	ft_memset(&prg, 0, sizeof(prg));
	i = parse_flag(&f, av, ac, 1);
	prg.target = av[i] ? av[i] : "a.out";
	prg.proc = &nm;
	while (prg.target)
	{
		err = ofile_create(&prg);
		i++;
		prg.target = av[i];
	}
	return (err);
}
