/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 21:01:48 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/30 13:55:54 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "alloc_list.h"
#include "alloc_vector.h"
#include "utils_hook.h"
#include <stdio.h>

void add_alloc_list(t_alloc_list **list, void *ptr, size_t size,
					void *const trace[])
{
	t_alloc_list *new_alloc;

	if (!(new_alloc = malloc(sizeof(t_alloc_list))))
		return;
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
			return;
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

void clear_alloc_list(t_alloc_list **list)
{
	t_alloc_list *tmp = *list;
	t_alloc_list *next;

	while (tmp)
	{
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	*list = NULL;
}

void print_alloc_list(t_alloc_list *list, int fd)
{
	t_alloc_vector vector;

	init_alloc_vector(&vector);
	t_alloc_list *tmp = list;
	while (tmp)
	{
		t_alloc alloc;

		alloc.size = tmp->size;
		route_copy(alloc.route, tmp->trace);

		t_alloc *found = find_alloc_vector(&vector, alloc.route);
		if (found)
		{
			found->size += alloc.size;
			found->iteration++;
		}
		else
			push_back_vector(&vector, &alloc);
		tmp = tmp->next;
	}

	dprintf(fd, "leaked block count: %zd\n", size_alloc_list(list));
	dprintf(fd, "-------------------------------------------------------------------------------\n");
	dprintf(fd, "|                                LEAKS SUMMARY\
                                |\n");
	dprintf(fd, "-------------------------------------------------------------------------------\n");
	dprintf(fd, "|%-25s|%-25s|%-25s|\n", "route", "allocated size", "count");
	dprintf(fd, "-------------------------------------------------------------------------------\n");
	for (size_t i = 0; i < vector.size; i++)
	{
		t_alloc alloc = vector.data[i];
		size_t i = 0;
		dprintf(fd, "|%-25s|%-25zi|%-25zi|\n", get_func_name(alloc.route[i++]), alloc.size, alloc.iteration);
		for (; alloc.route[i]; i++)
			dprintf(fd, "|%-25s|%-25s|%-25s|\n", get_func_name(alloc.route[i]), "", "");
		dprintf(fd, "-------------------------------------------------------------------------------\n");
	}
	clear_alloc_vector(&vector);
}
