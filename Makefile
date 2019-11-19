# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dbaffier <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/24 00:18:28 by dbaffier          #+#    #+#              #
#    Updated: 2019/11/19 16:30:23 by dbaffier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NM = ft_nm
OTOOL = ft_otool

CFLAGS += -Wall -Werror -Wextra

LIBFT_PATH = libft
LIBFT_LIB = $(LIBFT_PATH)/libft.a
LIBFT_LINK = -L$(LIBFT_PATH) -lft

INC_DIR = inc/
INCS = -I$(LIBFT_PATH)/inc -I$(INC_DIR)

OBJS_NM_DIR = objs_nm/
OBJS_NM = $(addprefix $(OBJS_NM_DIR), $(SRCS_NM:.c=.o))

OBJS_OT_DIR = objs_otool/
OBJS_OT = $(addprefix $(OBJS_OT_DIR), $(SRCS:.c=.o))

SRCS_NM_DIR = nm/
SRCS_NM = main.c					\
		  ofile_process.c			\
		  ofile_member.c			\
		  ofile_member_clear.c		\
		  ofile_unmap.c				\
		  member_tools.c			\
		  err_file.c				\
		  ft_archive.c				\
		  ft_mach_o.c				\
		  ft_fat.c					\
		  swap_bytes.c				\
		  print_fat.c				\
		  swap_header.c				\
		  byte_sex.c				\
		  member_debug.c			\
		  nm.c						\
		  swap_bytes_h.c			\
		  symbol.c					\
		  print.c					\
		  symbol_type.c				\

SRCS_OT_DIR = otool/
SRCS_OT = 

all: $(OBJS_NM_DIR) $(OBJS_OT_DIR) $(LIBFT_LIB) $(NM)

nm: $(OBJS_NM_DIR) $(LIBFT_LIB) $(NM)

$(OBJS_NM_DIR):
	@mkdir -p $@

$(OBJS_OT_DIR):
	@mkdir -p $@

$(LIBFT_LIB):
	@make -C $(LIBFT_PATH)

$(NM): $(OBJS_NM)
	gcc -fsanitize=address $^ -o $@ $(LIBFT_LINK)

$(OBJS_NM_DIR)%.o: $(SRCS_NM_DIR)%.c
	gcc $(CFLAGS) -o $@ -c $< $(INCS)

clean:
	@make -C $(LIBFT_PATH) clean
	rm -r $(OBJS_NM_DIR)
	rm -r $(OBJS_OT_DIR)

fclean: clean
	rm -f $(LIBFT_LIB)
	rm -f $(NM)
	rm -f $(OTOOL)

re: fclean all
