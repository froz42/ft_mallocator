# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/14 10:00:31 by tmatis            #+#    #+#              #
#    Updated: 2021/11/01 23:26:12 by tmatis           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#                                     CONFIG                                   #
################################################################################


NAME	= libmallocator.a
CC 		= clang
CFLAGS	= -Wall -Wextra -Werror -g
DFLAGS	= -MMD -MF $(@:.o=.d)

################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

FILE_EXTENSION	= .c

SRCS_PATH		= ./alloc_hook

SRCS			= alloc_hook.c utils_hook.c alloc_list.c alloc_vector.c

SRCS_TEST		= demo_main.c

################################################################################
#                                  Makefile  objs                              #
################################################################################

SHELL := /bin/bash


OBJS				= $(addprefix objs/, ${SRCS:$(FILE_EXTENSION)=.o})
OBJS_TEST			= $(addprefix objs/, ${SRCS_TEST:$(FILE_EXTENSION)=.o})
DEPS				= $(addprefix objs/, ${SRCS:$(FILE_EXTENSION)=.d})

################################################################################
#                                 Makefile logic                               #
################################################################################

all: $(NAME)

-include $(DEPS)
$(NAME):	${OBJS}
			ar -rcs $@ ${OBJS}

objs/%.o: 	$(SRCS_PATH)/%$(FILE_EXTENSION)
			@mkdir -p $(dir $@)
			$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

objs/%.o: 	%$(FILE_EXTENSION)
			@mkdir -p $(dir $@)
			$(CC) $(CFLAGS) -c $< -o $@		

clean:		
			@rm -rf objs

fclean:		clean

re:			fclean all

malloc_test: ${OBJS_TEST}
			$(CC) $(CFLAGS) -fsanitize=undefined -rdynamic -o $@ ${OBJS_TEST} -L. -lmallocator

.PHONY:		all clean fclean re header
