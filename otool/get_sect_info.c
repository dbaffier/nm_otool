/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_sect_info.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 18:15:58 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/10 18:47:03 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_otool.h"


static void	lc_seg_32(t_ofile *of, t_data *data, struct load_command *lc)
{
	struct segment_command	sg;
	struct section			s;
	char					*p;
	uint32_t				i;

	ft_memcpy((char *)&sg, (char *)lc, sizeof(struct segment_command));
	if ((data->mh_filetype = MH_OBJECT && sg.segname[0] == '\0') ||
			ft_strncmp(sg.segname, SEG_TEXT, sizeof(sg.segname)) == 0)
		data->seg_addr = sg.vmaddr;
	p = (char *)lc + sizeof(struct segment_command);
	i = -1;
	while (++i < sg.nsects)
	{
		ft_memcpy((char *)&s, p, sizeof(struct section));
		if(ft_strncmp(s.sectname, sectname, sizeof(s.sectname)) == 0 &&
				ft_strncmp(s.segname, segname, sizeof(s.segname)) == 0)
		{
			found = TRUE;
			cmd = LC_SEGMENT;

		}
	}
}

void		get_sect_info(t_ofile *of, t_data *data)
{
	struct load_command	*lc
		struct load_command	l;
	int					cond;
	uint32_t			i;

	i = 0;
	while (i < data->mh_ncmds)
	{
		ft_memcpy((char *)&l, (char *)lc, sizeof(struct load_command));
		if (l.cmd == LC_SEGMENT)
		{
			ft_memcpy((char *)&sg
					}
					}

					}
					}
