/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 13:28:09 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/25 14:37:41 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>
#include <errno.h>
#include <fcntl.h>
#include "malloc_hook.h"
#include "vector.h"

int g_malloc_hook_active = 1;
int g_free_hook_active = 1;

vector_t g_malloc_hook_vector;

int g_malloc_vector_init = 0;
int g_at_exit_hook_active = 0;

int g_malloc_count = 0;
int g_free_count = 0;


void at_exit_hook(void)
{
	g_malloc_hook_active = 0;
	g_free_hook_active = 0;
	print_vector(&g_malloc_hook_vector);
	free_vector(&g_malloc_hook_vector);
	printf("%d mallocs, %d frees\n", g_malloc_count, g_free_count);
}

void *my_malloc_hook(size_t size, void *caller)
{
	void *result;

	g_malloc_hook_active = 0;
	//static int	fail_counter = 0;

	/*
	if (++fail_counter == FAIL)
	{
		errno = ENOMEM;
		fail_counter = 0;
		result = NULL;
	}
	else
	*/
	
	if (!g_at_exit_hook_active)
	{
		g_at_exit_hook_active = 1;
		atexit(at_exit_hook);
	}


	size_t caller_address = (size_t)(&_end) - (size_t)caller;
	if (DRY_RUN)
	{
		if (!g_malloc_vector_init)
		{
			init_vector(&g_malloc_hook_vector);
			g_malloc_vector_init = 1;
		}
		if (!find_vector(&g_malloc_hook_vector, caller_address))
			push_back_vector(&g_malloc_hook_vector, caller_address);
	}
	//printf("malloc called at: %#lx\n", caller_address);
	result = malloc(size);
	g_malloc_count++;
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


void free(void *ptr)
{
	void *caller;

	caller = __builtin_return_address(0);
	if (g_free_hook_active)
		g_free_count++;	
	g_free_hook_active = 0;
	__libc_free(ptr);
	g_free_hook_active = 1;
}
