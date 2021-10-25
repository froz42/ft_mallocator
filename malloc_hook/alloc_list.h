/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_list.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 20:24:09 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/25 20:58:15 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOC_LIST_H
#define ALLOC_LIST_H

#include <stdlib.h>

typedef struct s_alloc_list
{
	void *ptr;
	size_t size;
	void *caller;
	struct s_alloc_list *next;
} t_alloc_list;

typedef struct
{
	const char *dli_fname;
	void *dli_fbase;
	const char *dli_sname;
	void *dli_saddr;
} Dl_info;

int dladdr(void *address, Dl_info *dlip);

void push_list(t_alloc_list **list, void *ptr, size_t size, void *caller);
t_alloc_list *find_list(t_alloc_list *list, void *ptr);
void pop_list(t_alloc_list **list, void *ptr);
void print_list(t_alloc_list *list);
void clear_list(t_alloc_list **list);

#endif
