/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 13:28:09 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/25 22:11:21 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include <errno.h>
#include <fcntl.h>
#include "malloc_hook.h"
#include "vector.h"
#include <string.h>
#include <dlfcn.h>
#include "alloc_list.h"


int g_malloc_hook_active = 1;

vector_t g_malloc_hook_vector;

int g_malloc_vector_init = 0;
int g_at_exit_hook_active = 0;

int g_malloc_count = 0;
int g_free_count = 0;

size_t g_fetch;
int g_fetch_active = 0;

t_alloc_list *g_alloc_list = NULL;

void setup_g_fetch(void)
{
	int fd = open("./addr.tmp", O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	char buffer[100];
	size_t read_bytes = read(fd, buffer, sizeof(buffer) - 1);
	if (read_bytes < 0)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
	buffer[read_bytes] = '\0';

	g_fetch = strtoul(buffer, NULL, 16);
	close(fd);
}

void at_exit_hook(void)
{
	g_malloc_hook_active = 0;

	printf("malloc: %d\n", g_malloc_count);
	printf("free: %d\n", g_free_count);
	if (g_fetch == 0)
	{
		print_vector(&g_malloc_hook_vector);
		free_vector(&g_malloc_hook_vector);
	}
	if (g_alloc_list)
	{
		print_list(g_alloc_list);
		clear_list(&g_alloc_list);
	}
}

void *my_malloc_hook(size_t size, void *caller)
{
	void *result;

	g_malloc_hook_active = 0;

	if (!g_at_exit_hook_active)
	{
		g_at_exit_hook_active = 1;
		atexit(at_exit_hook);
	}

	if (!g_fetch_active)
	{
		g_fetch_active = 1;
		setup_g_fetch();
	}

	size_t caller_address = (size_t)(&_end) - (size_t)caller;

	if (g_fetch == 0)
	{
		if (!g_malloc_vector_init)
		{
			init_vector(&g_malloc_hook_vector);
			g_malloc_vector_init = 1;
		}
		pair_t pair;
		pair.data = caller_address;
		pair.caller = caller;
		if (!find_vector(&g_malloc_hook_vector, &pair))
		{
			if ((caller_address & 0xffff000000000000) == 0)
				push_back_vector(&g_malloc_hook_vector, &pair);
		}
		result = malloc(size);
	}
	else
	{
		if (g_fetch == caller_address)
			// block malloc
			result = NULL;
		else
			result = malloc(size);
	}
	if (result != NULL)
	{
		push_list(&g_alloc_list, result, size, caller);
		g_malloc_count++;
	}
	g_malloc_hook_active = 1;
	return (result);
}

void *malloc(size_t size)
{
	void *caller;

	caller = __builtin_return_address(0);
	if (g_malloc_hook_active)
		return (my_malloc_hook(size, caller));
	return (__libc_malloc(size));
}

void *calloc(size_t nmemb, size_t size)
{
	void *caller;

	caller = __builtin_return_address(0);
	if (g_malloc_hook_active)
	{
		char *result = my_malloc_hook(nmemb * size, caller);
		if (result)
			memset(result, 0, nmemb * size);
		return (result);
	}
	return (__libc_calloc(nmemb, size));
}

void my_free_hook(void *ptr)
{
	g_malloc_hook_active = 0;
	pop_list(&g_alloc_list, ptr);
	g_free_count++;
	g_malloc_hook_active = 1;
}


void free(void *ptr)
{
	if (g_malloc_hook_active)
		my_free_hook(ptr);
	__libc_free(ptr);
}
