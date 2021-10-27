/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 21:01:48 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/27 17:50:30 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "alloc_list.h"
#include "utils_hook.h"
#include <stdio.h>

void add_alloc_list(t_alloc_list **list, void *ptr, size_t size,
		void * const trace[20])
{
	t_alloc_list *new_alloc;

	if (!(new_alloc = malloc(sizeof(t_alloc_list))))
		return ;
	new_alloc->ptr = ptr;
	new_alloc->size = size;

	route_copy(new_alloc->trace, trace);
	
	new_alloc->next = NULL;
	
	if (*list == NULL)
		*list = new_alloc;
	else
	{
		t_alloc_list *tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_alloc;
	}
}

void remove_alloc_list(t_alloc_list **list, void *ptr)
{
	t_alloc_list *tmp = *list;
	t_alloc_list *prev = NULL;

	while (tmp)
	{
		if (tmp->ptr == ptr)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*list = tmp->next;
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

size_t size_alloc_list(t_alloc_list *list)
{
	size_t size = 0;
	while (list)
	{
		size++;
		list = list->next;
	}
	return (size);
}

void print_alloc_list(t_alloc_list *list, int fd)
{
	dprintf(fd, "leaked block count: %zd\n", size_alloc_list(list));
	dprintf(fd, "--------------------------------------------------\
-----------------------------\n");
	dprintf(fd, "|%25s|%25s|%25s|\n", "route", "size", "pointer");
	dprintf(fd, "--------------------------------------------------\
-----------------------------\n");
	while (list)
	{
		size_t route_size = 0;
		while (list->trace[route_size])
		{
			printf("%s\n",	get_func_name(list->trace[route_size]));
			route_size++;
		}
		

		

		list = list->next;
	}
}
