/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_archive.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 18:58:20 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/07 16:57:11 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int		ft_archive(t_ofile *of, void *addr, uint32_t magic, enum byte_sex e)
{
	(void)e;
	(void)magic;
	if (of->file_size >= SARMAG && strncmp(addr, ARMAG, SARMAG) == 0)
	{
		of->file_type = OFILE_ARCHIVE;
		of->object_addr = addr;
	}
	return (0);
}

int		process_archive(t_nm *nm_t, t_ofile *of)
{
	(void)nm_t;
	if (ofile_first_member(of) == 0)
	{
		ofile_member_debug(of);
		nm_t->processor(of, NULL, cookie);
	}
	return (1);
}
