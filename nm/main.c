/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 00:35:15 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/16 23:41:16 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "ft_ofile.h"

int main(int ac, char **av)
{
	t_prg		prg;
	int			err;
	size_t		i;

	ft_memset(&prg, 0, sizeof(prg));
	i = 1;
	prg.target = ac > 1 ? av[i] : "a.out";
	prg.proc = &nm;
	while (prg.target)
	{
		err = ofile_create(&prg);
		i++;
		prg.target = av[i];
	}
	return (err);
}
