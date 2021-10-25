# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/14 10:00:31 by tmatis            #+#    #+#              #
#    Updated: 2021/10/25 15:50:08 by tmatis           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#                                     CONFIG                                   #
################################################################################


NAME	= libmallocator.a
CC 		= clang
CFLAGS	= -Wall -Wextra -Werror -g
DFLAGS	= -MMD -MF $(@:.o=.d)

FETCH_RUN = 0

################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

FILE_EXTENSION	= .c

SRCS_PATH		= ./malloc_hook

INCLUDE_PATH	= ./

SRCS			= malloc_hook.c vector.c

################################################################################
#                                  Makefile  objs                              #
################################################################################

SHELL := /bin/bash


OBJS				= $(addprefix objs/, ${SRCS:$(FILE_EXTENSION)=.o})
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
			$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@ -I$(INCLUDE_PATH) -D FETCH_RUN=$(FETCH_RUN)

clean:		
			@rm -rf objs

fclean:		clean
			@rm -rf $(NAME)

re:			fclean all

.PHONY:		all clean fclean re header
