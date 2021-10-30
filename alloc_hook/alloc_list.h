/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_list.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 20:54:59 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/30 13:07:33 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOC_LIST_H
# define ALLOC_LIST_H

#include <stdlib.h>


typedef struct		s_alloc_list
{
	void			*ptr;
	size_t			size;
	void			*trace[100];
	size_t			iterations;
	
	struct s_alloc_list	*next;
}					t_alloc_list;

void add_alloc_list(t_alloc_list **list, void *ptr, size_t size, void * const trace[]);
void remove_alloc_list(t_alloc_list **list, void *ptr);
size_t size_alloc_list(t_alloc_list *list);
void print_alloc_list(t_alloc_list *list, int fd);
void clear_alloc_list(t_alloc_list **list);


#endif
