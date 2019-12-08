/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_loadcmds.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 23:09:44 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/08 23:22:28 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"

void	print_loadcmds(t_ofile *of, t_data *data)
{
	(void)of;
	(void)data;
	int			swapped;

	swapped = get_host_byte_sex() != of->object_byte_sex;
	ft_printf("%d\n", swapped);

}
