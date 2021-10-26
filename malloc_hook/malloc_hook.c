/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 13:28:09 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/26 19:45:00 by tmatis           ###   ########.fr       */
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
#include "malloc_hook.h"


int g_malloc_hook_active = 1;

int g_at_exit_hook_active = 0;

int g_fd_out = -1; // ./ft_mallocator/res.tmp


void setup_g_fetch(void)
{
}

void setup_g_fd_out(void)
{
	g_fd_out = open("./ft_mallocator/res.tmp", O_WRONLY | O_CREAT, 0644);
	if (g_fd_out < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
}

void at_exit_hook(void)
{
	g_malloc_hook_active = 0;
	
}


void *my_malloc_hook(size_t size, void *caller)
{
	void *result;

	g_malloc_hook_active = 0;

	if (g_fd_out == -1)
		setup_g_fd_out();

	if (!g_at_exit_hook_active)
	{
		g_at_exit_hook_active = 1;
		atexit(at_exit_hook);
	}
	g_malloc_hook_active = 1;
	return (NULL);
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
	g_malloc_hook_active = 1;
}


void free(void *ptr)
{
	if (g_malloc_hook_active)
		my_free_hook(ptr);
	__libc_free(ptr);
}
