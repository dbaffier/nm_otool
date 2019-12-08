/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 17:31:26 by dbaffier          #+#    #+#             */
/*   Updated: 2019/12/08 18:03:30 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_OTOOL_H
# define FT_OTOOL_H

typedef struct		s_flags
{
	unsigned int	a;
}					t_flags;

void		processor(t_ofile *of, char *arch_name, void *cookie);

#endif
