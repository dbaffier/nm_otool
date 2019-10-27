/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_valist.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/31 20:22:33 by dbaffier          #+#    #+#             */
/*   Updated: 2019/06/30 16:35:16 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void			(*get_arg_func(t_format *data))(void *dst, va_list *args)
{
	static t_getarg tab[] = {GET_NULL, GET_C, GET_S, GET_F, GET_G, GET_GF,
		GET_GS, GET_GC, GET_U, GET_X, GET_GX, GET_PTR, GET_O, GET_M, GET_B,
		GET_I, GET_D, GET_K, GET_STAR, GET_R, GET_GR, GET_GU, GET_GO, GET_GD
	};
	size_t			i;

	i = 0;
	while (i < sizeof(tab) / sizeof(tab[0]))
	{
		if (*data->tail == tab[i].type)
			return (tab[i].func);
		i++;
	}
	return (NULL);
}

void			create_node(t_va_list **head, t_format *data, int type)
{
	t_va_list	*new;
	t_va_list	*q;

	if (!(new = ft_memalloc(sizeof(t_va_list))))
		return ;
	new->type = type;
	new->func = get_arg_func(data);
	new->next = NULL;
	new->id = 1;
	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	q = *head;
	while (q->next)
		q = q->next;
	new->id = q->id + 1;
	q->next = new;
}

t_va_list		*get_lstarg(t_va_list *head, short id)
{
	while (head)
	{
		if (head->id == id)
			return (head);
		head = head->next;
	}
	return (NULL);
}

void			get_arg_n(t_format *data, void *dst)
{
	va_list			arg_copy;
	short			i;
	short			arg_num;
	t_va_list		*tmp;

	tmp = NULL;
	va_copy(arg_copy, data->copy);
	i = 1;
	arg_num = data->flag.id > 0 ? data->flag.id : data->id;
	data->id = data->id + 1;
	while (i < arg_num)
	{
		tmp = get_lstarg(data->lst, i);
		if (tmp == NULL)
			return ;
		tmp->func(NULL, &arg_copy);
		i++;
	}
	tmp = get_lstarg(data->lst, i);
	if (tmp == NULL)
		return ;
	tmp->func(dst, &arg_copy);
	va_end(arg_copy);
}

void			get_valist(t_format *data)
{
	int			n;
	t_va_list	*vl;

	vl = NULL;
	data->tail++;
	if (!ft_isdigit(*data->tail))
		return ;
	n = 0;
	while (ft_isdigit(*data->tail))
	{
		n = n * 10 + (*data->tail - 48);
		data->tail++;
	}
	data->flag.id = (short)n;
}
