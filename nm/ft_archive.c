/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_archive.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 18:58:20 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/19 16:34:15 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ofile.h"

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

int		process_archive(t_prg *prg, t_ofile *of)
{
	if (ofile_first_member(of) == 0)
	{
		//ofile_member_debug(of);
		prg->proc(of, NULL, NULL);
	//	getchar();
	}
	while (ofile_next_member(of) == 0)
	{
	//	ofile_member_debug(of);
		prg->proc(of, NULL, NULL);
		//getchar();
	}
	return (1);
}
