/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ofile_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 23:28:11 by dbaffier          #+#    #+#             */
/*   Updated: 2019/10/27 02:01:02 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int	ofile_type(t_ofile *ofile)
{

		
}

static int	ofile_map(t_nm *nm_t, t_ofile *ofile)
{
	struct stat		sb;

	if ((ofile->fd = open(nm_t->target, O_RDONLY)) == -1)
		return (ERR_OPEN);
	if (fstat(ofile->fd, &sb) == -1)
		return (ERR_STAT);
	nm_t->file_size = sb.st_size;
	ofile->file_addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, ofile->fd, 0);
	ofile->file_size = sb.st_size;
	if (ofile->addr == MAP_FAILED)
		return (ERR_MAP);
	if ((ofile->file_name = ft_strdup(nm_t->target)) == NULL)
		return (ERR_MALLOC);
	ofile_type(ofile);
	return (0);
}

int			ofile_process(t_nm *nm_t)
{
	struct t_ofile	ofile;

	if ((ret = ofile_map(nm_t, &ofile)) > 0)
		return (ret);
	nm_routine(nm_t, nm_t->addr);
	close(ofile.fd);
	// free ofile;
	return (0);
}
