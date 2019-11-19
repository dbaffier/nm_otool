/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lolz.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 15:03:08 by dbaffier          #+#    #+#             */
/*   Updated: 2019/11/16 15:29:48 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

long long	swap_long_long(long long ll);


long long
SWAP_LONG_LONG(
long long ll)
{
	union {
	    char c[8];
	    long long ll;
	} in, out;
	in.ll = ll;
	out.c[0] = in.c[7];
	out.c[1] = in.c[6];
	out.c[2] = in.c[5];
	out.c[3] = in.c[4];
	out.c[4] = in.c[3];
	out.c[5] = in.c[2];
	out.c[6] = in.c[1];
	out.c[7] = in.c[0];
	return(out.ll);
}

unsigned int swap_uint32(unsigned int val)
{
	printf("---%#x\n", (val >> 8) & 0xFF00FF);
	printf("--%#x\n", (val >> 8) & 0xFF0000);
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF); 
	//printf("---%#x\n", (val >> 8) & 0xFF00FF);
	//printf("--%#x\n", (val >> 8) & 0xFF0000);
    return (val << 16) | (val >> 16);
}

int main(void)
{
	unsigned int		i = 6864;
	unsigned long long	ll = 68741545641;
	printf("%#x\n", i);
	printf("%#x\n", swap_uint32(i));
	printf("%#llx\n", ll);
	printf("%#llx\n", swap_long_long(ll));
	return (0);
}
