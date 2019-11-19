/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   of_member_clear.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 22:21:46 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/14 13:49:49 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ofile.h"

void	ofile_member_clear(t_ofile *of)
{
	of->member_offset = 0;
	of->member_addr = NULL;
	of->member_size = 0;
	of->member_ar_hdr = NULL;
	of->member_name = NULL;
	of->member_name_size = 0;
	of->member_type = OFILE_UNKNOWN;
	of->object_addr = NULL;
	of->object_size = 0;
	of->object_byte_sex = UNKNOWN_BYTE_SEX;
	of->mh = NULL;
	of->load_commands = NULL;
}

void	ofile_object_clear(t_ofile *of)
{
	of->member_type = OFILE_UNKNOWN;
	of->object_addr = NULL;
	of->object_size = 0;
	of->object_byte_sex = UNKNOWN_BYTE_SEX;
	of->mh = NULL;
	of->load_commands = NULL;
}

