/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 13:28:09 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/27 13:13:10 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <dlfcn.h>
#include "utils_hook.h"
#include "alloc_list.h"
#include "alloc_vector.h"
#include "malloc_hook.h"

#define LEAK_PATH "./res.tmp"
#define ROUTES_PATH "./routes.tmp"

int g_malloc_hook_active = 1;

int g_at_exit_hook_active = 0;

void *g_route[20];
int g_route_setup = 0;

int g_fetch_mode = 1;

t_alloc_vector g_alloc_vector;
int g_alloc_vector_setup = 0;

t_alloc_list *g_alloc_list = NULL;

void at_exit_hook(void)
{
	g_malloc_hook_active = 0;
	int routes_fd = open(ROUTES_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (routes_fd < 0)
	{
		perror("at_exit_hook");
		exit(EXIT_FAILURE);
	}
	//dprintf(g_fd_out, "leaked blocks: %zi\n", size_alloc_list(g_alloc_list));
	print_alloc_vector(&g_alloc_vector, routes_fd);
}

void get_backtrace(void *trace[20])
{
	void *array[23];
	size_t size;

	size = backtrace(array, 23);
	size_t y = 0;
	for (size_t i = 2; i < (size - 2); i++)
	{
		trace[y++] = array[i];
	}
	trace[y] = NULL;
}

void *alloc_hook(size_t size, void *caller)
{
	char *result = NULL;

	g_malloc_hook_active = 0;
	
	if (!g_at_exit_hook_active)
	{
		g_at_exit_hook_active = 1;
		atexit(at_exit_hook);
	}

	if (g_fetch_mode && !g_alloc_vector_setup)
	{
		g_alloc_vector_setup = 1;
		init_alloc_vector(&g_alloc_vector);
	}

	size_t caller_address = (size_t)(&_end) - (size_t)caller;
	if ((caller_address & 0xffff000000000000) == 0)
	{
		void *route[20];

		get_backtrace(route);

		result = malloc(size);
		add_alloc_list(&g_alloc_list, result, size, route);
		if (g_fetch_mode)
		{
			t_alloc *found = find_alloc_vector(&g_alloc_vector, route);
			if (!found)
			{
				t_alloc alloc;
				
				route_copy(alloc.route, route);
				push_back_vector(&g_alloc_vector, &alloc);
			}
			else
				found->iteration++;
		}
	}
	else
		result = malloc(size);

	g_malloc_hook_active = 1;

	return (result);
}

void *malloc(size_t size)
{
	void *caller;

	caller = __builtin_return_address(0);
	if (g_malloc_hook_active)
		return (alloc_hook(size, caller));
	return (__libc_malloc(size));
}

void *calloc(size_t nmemb, size_t size)
{
	void *caller;

	caller = __builtin_return_address(0);
	if (g_malloc_hook_active)
	{
		char *result = alloc_hook(nmemb * size, caller);
		if (result)
			memset(result, 0, nmemb * size);
		return (result);
	}
	return (__libc_calloc(nmemb, size));
}

void my_free_hook(void *ptr)
{
	(void)ptr;
	g_malloc_hook_active = 0;
	remove_alloc_list(&g_alloc_list, ptr);
	g_malloc_hook_active = 1;
}

void free(void *ptr)
{
	if (g_malloc_hook_active)
		my_free_hook(ptr);
	__libc_free(ptr);
}
