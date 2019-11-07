/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ofile_unmap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 19:34:12 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/04 22:47:39 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void		ofile_unmap(t_nm *nm_t, t_ofile *of)
{
	(void)nm_t;
	if (of->file_addr)
		munmap(of->file_addr, of->file_size);
	if (of->file_name)
		free(of->file_name);
	ft_memset(of, '\0', sizeof(t_ofile));
}
