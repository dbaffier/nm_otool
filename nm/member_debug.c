/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   member_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 15:22:17 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/07 16:46:17 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	ofile_member_debug(t_ofile *of)
{
	ft_printf("File addr : %p\n", of->file_addr);
	ft_printf("Member addr [%p]\n", of->member_addr);
	ft_printf("\toffset : [%u]\n", of->member_offset);
	ft_printf("\tsize : [%u]\n", of->member_size);
	ft_printf("\ttype : [%d]\n", of->member_type);
	ft_printf("\ttype : [%d] == [%d]\n", of->member_type, OFILE_Mach_O);

}
