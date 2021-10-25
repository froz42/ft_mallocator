/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 20:25:29 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/25 21:10:22 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "alloc_list.h"
#include <stdio.h>

char const *get_func_name(void *addr)
{
	Dl_info info;
	if (dladdr(addr, &info) && info.dli_sname)
		return (info.dli_sname);
	else
		return ("unkwow");
}

void push_list(t_alloc_list **list, void *ptr, size_t size, void *caller)
{
	t_alloc_list *new;

	new = (t_alloc_list *)malloc(sizeof(t_alloc_list));
	if (new == NULL)
		exit(EXIT_FAILURE);
	new->ptr = ptr;
	new->size = size;
	new->caller = caller;
	new->next = NULL;
	if (*list == NULL)
		*list = new;
	else
	{
		while ((*list)->next != NULL)
			*list = (*list)->next;
		(*list)->next = new;
	}
}

t_alloc_list *find_list(t_alloc_list *list, void *ptr)
{
	while (list != NULL)
	{
		if (list->ptr == ptr)
			return (list);
		list = list->next;
	}
	return (NULL);
}

void pop_list(t_alloc_list **list, void *ptr)
{
	t_alloc_list *tmp;

	if (*list == NULL)
		return;
	if ((*list)->ptr == ptr)
	{
		tmp = *list;
		*list = (*list)->next;
		free(tmp);
		return;
	}
	while ((*list)->next != NULL)
	{
		if ((*list)->next->ptr == ptr)
		{
			tmp = (*list)->next;
			(*list)->next = (*list)->next->next;
			free(tmp);
			return;
		}
		*list = (*list)->next;
	}
}

void print_list(t_alloc_list *list)
{
	while (list != NULL)
	{
		printf("%p\t%zu\t%s\n", list->ptr, list->size, get_func_name(list->caller));
		list = list->next;
	}
}

void clear_list(t_alloc_list **list)
{
	t_alloc_list *tmp;

	while (*list != NULL)
	{
		tmp = *list;
		*list = (*list)->next;
		free(tmp);
	}
}
