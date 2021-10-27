/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_list.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 20:54:59 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/27 13:07:15 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOC_LIST_H
# define ALLOC_LIST_H

#include <stdlib.h>


typedef struct		s_alloc_list
{
	void			*ptr;
	size_t			size;
	void			*trace[20];
	
	struct s_alloc_list	*next;
}					t_alloc_list;

void add_alloc_list(t_alloc_list **list, void *ptr, size_t size, void * const trace[20]);
void remove_alloc_list(t_alloc_list **list, void *ptr);
size_t size_alloc_list(t_alloc_list *list);


#endif
