/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 04:33:29 by dbaffier          #+#    #+#             */
/*   Updated: 2019/07/10 23:48:05 by dbaffier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdlib.h>
# include <stdarg.h>
# include "libft.h"
# include "converter.h"

# define BUFF_SIZE 1024

# define OPT_MINUS (1 << 0)
# define OPT_PLUS (1 << 1)
# define OPT_ZERO (1 << 2)
# define OPT_SPACE (1 << 3)
# define OPT_HASH (1 << 4)
# define OPT_PREC (1 << 5)
# define OPT_STAR (1 << 6)
# define OPT_HSTAR (1 << 7)
# define OPT_APS (1 << 8)

# define OPT_H (1 << 0)
# define OPT_HH (1 << 1)
# define OPT_L (1 << 2)
# define OPT_LL (1 << 3)
# define OPT_J (1 << 4)
# define OPT_Z (1 << 5)

typedef struct			s_flag
{
	char				star;
	short				id;
	int					opts;
	int					size_modifier;
	size_t				precision;
	size_t				width;
}						t_flag;

typedef struct			s_va_list
{
	int					type;
	void				(*func)(void *ptr, va_list *args);
	int					id;
	struct s_va_list	*next;
}						t_va_list;

typedef struct			s_format
{
	unsigned int	pos;
	unsigned int	tmp_pos;
	int				len;
	int				ret;
	int				*fd;
	short			id;
	va_list			arg;
	va_list			copy;
	char			*tail;
	char			*head;
	void			(*print_buff)(struct s_format *data);
	char			buff[BUFF_SIZE];
	t_flag			flag;
	t_va_list		*lst;
}						t_format;

typedef struct			s_color
{
	char				*color;
	char				*code;
}						t_color;

typedef struct			s_convert
{
	char				type;
	void				(*func)(t_format *data);
}						t_convert;

typedef struct			s_getarg
{
	int					type;
	void				(*func)(void *ptr, va_list *args);
}						t_getarg;

# define GET_NULL	{'\0', getarg_null}
# define GET_B		{'b', getarg_uint}
# define GET_C		{'c', getarg_int}
# define GET_D		{'d', getarg_int}
# define GET_F		{'f', getarg_float}
# define GET_G		{'g', getarg_float}
# define GET_I		{'i', getarg_int}
# define GET_K		{'k', getarg_uint}
# define GET_O		{'o', getarg_uintmaxt}
# define GET_PTR	{'p', getarg_uintmaxt}
# define GET_R		{'r', getarg_ptr}
# define GET_S		{'s', getarg_ptr}
# define GET_U		{'u', getarg_uintmaxt}
# define GET_X		{'x', getarg_uintmaxt}
# define GET_GC		{'C', getarg_wchar}
# define GET_GD		{'D', getarg_int}
# define GET_GF		{'F', getarg_float}
# define GET_GO		{'O', getarg_uintmaxt}
# define GET_GR		{'R', getarg_ptr}
# define GET_GS		{'S', getarg_wstr}
# define GET_GU		{'U', getarg_uintmaxt}
# define GET_GX		{'X', getarg_uintmaxt}
# define GET_M		{'%', getarg_null}

# define GET_STAR	{'*', getarg_int}
# define CONVERT_B	{'b', convert_b}
# define CONVERT_C	{'c', convert_c}
# define CONVERT_D	{'d', convert_d}
# define CONVERT_F	{'f', convert_f}
# define CONVERT_G	{'g', convert_f}
# define CONVERT_I	{'i', convert_d}
# define CONVERT_K	{'k', convert_k}
# define CONVERT_O	{'o', convert_o}
# define CONVERT_P	{'p', convert_p}
# define CONVERT_R	{'r', convert_r}
# define CONVERT_S	{'s', convert_s}
# define CONVERT_U	{'u', convert_u}
# define CONVERT_X	{'x', convert_x}
# define CONVERT_Z	{'z', convert_z}
# define CONVERT_GC	{'C', convert_gc}
# define CONVERT_GD	{'D', convert_d}
# define CONVERT_GF	{'F', convert_f}
# define CONVERT_GO	{'O', convert_o}
# define CONVERT_GR	{'R', convert_gr}
# define CONVERT_GS	{'S', convert_gs}
# define CONVERT_GU	{'U', convert_u}
# define CONVERT_GX	{'X', convert_gx}
# define CONVERT_M	{'%', convert_m}

# define WHITE	{"WHITE",	"\033[0m"}
# define BLACK	{"BLACK",	"\033[30m"}
# define RED	{"RED",		"\033[31m"}
# define GREEN	{"GREEN",	"\033[32m"}
# define YELLOW	{"YELLOW",	"\033[33m"}
# define BLUE	{"BLUE",	"\033[34m"}
# define PINK	{"PINK",	"\033[35m"}
# define CYAN	{"CYAN",	"\033[36m"}
# define GREY	{"GREY",	"\033[37m"}

/*
** src/converter/ -- convert_{c, s, p, d, i, o, u, x, X, f}.c
*/

void					convert_c(t_format *data);
void					convert_s(t_format *data);
void					convert_p(t_format *data);
void					convert_d(t_format *data);
void					convert_i(t_format *data);
void					convert_o(t_format *data);
void					convert_u(t_format *data);
void					convert_x(t_format *data);
void					convert_f(t_format *data);
void					convert_gx(t_format *data);
void					convert_gc(t_format *data);
void					convert_gs(t_format *data);
void					convert_z(t_format *data);
void					convert_b(t_format *data);
void					convert_r(t_format *data);
void					convert_gr(t_format *data);
void					convert_k(t_format *data);
void					convert_m(t_format *data);
void					conv_star(t_format *data);

/*
** src/ -- buff_func.c
*/
void					ft_parse(t_format *data);
void					getarg_null(void *dst, va_list *args);
void					getarg_float(void *ptr, va_list *args);
void					getarg_ptr(void *ptr, va_list *args);
void					getarg_int(void *ptr, va_list *args);
void					getarg_wchar(void *ptr, va_list *args);
void					getarg_wstr(void *ptr, va_list *args);
void					getarg_char(void *ptr, va_list *args);
void					getarg_uint(void *ptr, va_list *args);
void					getarg_long(void *dst, va_list *args);
void					getarg_uintmaxt(void *ptr, va_list *args);

/*
** src/converter/ --  convert_color.c
*/
int						get_color(t_format *data);
void					create_node(t_va_list **head, t_format *data, int type);

/*
** src/tools/ --
*/
int						get_converter(t_format *data);
void					get_flags(t_format *data);
void					get_varg(t_format *data);
void					get_arg_n(t_format *data, void *dst);
char					get_char_conv(t_format *data);

/*
** src/tools/ -- ft_utoa.c
*/
char					*ft_utoa(int n);
char					*ft_intmax_toa(intmax_t n);
void					ft_parse(t_format *data);
void					ft_preparse(t_format *data);

/*
** src/ -- ft_printf.c
*/
int						ft_printf(const char *str, ...);
int						ft_dprintf(int fd, const char *str, ...);

/*
** src/tools -- write_wchar.c
*/
size_t					ft_wstrlen(const wchar_t *s);
void					write_wchar(t_format *data, wchar_t c);
void					ft_putwchar(t_format *data, wchar_t c);
void					ft_putwstr(t_format *data, wchar_t *str, size_t n);
void					get_valist(t_format *data);

/*
** src/tools -- write_char.c
*/
void					write_str(t_format *data, char *str);
void					write_nstr(t_format *data, char *str, size_t len);
void					write_char(t_format *data, char c);
void					write_float(t_format *data, char c, int j);
void					write_nbr(t_format *data, uintmax_t base,
		uintmax_t n, char letter);

/*
** src/tools/ -- convert_base.c
*/
char					*ft_convert_base(intmax_t n,
		int base, size_t n_size, char letter);

/*
** src/tools/ -- convert_ubase.c
*/
char					*ft_convert_ubase(uintmax_t n,
		int base, size_t n_size, char letter);

/*
** src/tools/ -- float_to_string.c
*/
char					*float_to_string(t_format *data, t_convf *ptr);

/*
** src/tools/ -- len_func.c
*/
void					ft_doublelen(t_convf *ptr);
size_t					ft_intlen_base(intmax_t n, int base);
size_t					ft_uintlen_base(uintmax_t n, int base);

/*
** src/tools/ -- apostrophe_flag.c
*/
void					get_aps(t_format *data, char *str, unsigned int len);

/*
** src/tools/ -- check_mask.c
*/
int						msk(t_format *data, int mask);

/*
** src/tools/ -- get_specifier.c
*/
int						get_specifier(t_format *data);

#endif
