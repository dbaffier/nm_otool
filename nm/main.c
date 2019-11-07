/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 00:35:15 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/07 16:57:10 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "ft_nm.h"

int main(int ac, char **av)
{
	t_nm		nm_t;
	int			err;
	size_t		i;

	ft_memset(&nm_t, 0, sizeof(nm_t));
	i = 1;
	nm_t.target = ac > 1 ? av[i] : "a.out";
	while (nm_t.target)
	{
		err = ofile_create(&nm_t);
		i++;
		nm_t.target = av[i];
	}
	return (err);
}
