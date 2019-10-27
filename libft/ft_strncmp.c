/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmonier <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 12:34:46 by mmonier           #+#    #+#             */
/*   Updated: 2018/11/12 12:39:26 by mmonier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int i;

	i = 0;
	while (s1[i] == s2[i] && n > 0 && s1[i])
	{
		i++;
		n--;
	}
	return ((n == 0) ? 0 : (unsigned char)s1[i] - (unsigned char)s2[i]);
}
