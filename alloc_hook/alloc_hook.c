/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 13:28:09 by tmatis            #+#    #+#             */
/*   Updated: 2021/11/01 23:19:37 by tmatis           ###   ########.fr       */
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
#include "alloc_hook.h"

#define LEAK_PATH "./leaks.tmp"
#define ROUTES_PATH "./routes.tmp"
#define ADDRESS_PATH "./addresses.tmp"
#define ITER_PATH "./iteration.tmp"

int g_malloc_hook_active = 1;

int g_at_exit_hook_active = 0;

void *g_route[100] = {NULL};

int g_fetch_mode = -1;

t_alloc_vector g_alloc_vector;
int g_alloc_vector_setup = 0;

size_t g_iteration = 0;

t_alloc_list *g_alloc_list = NULL;

int setup_things = 8;

void at_exit_hook(void)
{
	g_malloc_hook_active = 0;

	//dprintf(g_fd_out, "leaked blocks: %zi\n", size_alloc_list(g_alloc_list));
	if (g_fetch_mode)
	{
		int routes_fd = open(ROUTES_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (routes_fd < 0)
		{
			perror("at_exit_hook");
			exit(EXIT_FAILURE);
		}
		print_alloc_vector(&g_alloc_vector, routes_fd);
		clear_alloc_vector(&g_alloc_vector);
	}

	int leaks_fd = open(LEAK_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (leaks_fd < 0)
	{
		perror("at_exit_hook");
		exit(EXIT_FAILURE);
	}
	print_alloc_list(g_alloc_list, leaks_fd);
	clear_alloc_list(&g_alloc_list);
}

void get_backtrace(void *trace[])
{
	void *array[103];
	size_t size;

	size = backtrace(array, 103);

	// search main pos
	
	size_t i;
	for (i = 0; i < size; i++)
	{
		if (strcmp(get_func_name(array[i]), "main") == 0)
			break;
	}

	if (i == size)
	{
		dprintf(STDERR_FILENO, "can't find main in backtrace, did you not setup -rdynamic flag ?\n");
		exit(EXIT_FAILURE);
	}
	size_t main_pos = size - i - 1;

	size_t y = 0;
	for (size_t i = 2; i < size - main_pos; i++)
		trace[y++] = array[i];
	trace[y] = NULL;
}

void setup_fetch(void)
{
	FILE *addresses;
	char *line = NULL;
	size_t len = 0;


	addresses = fopen(ADDRESS_PATH, "r");
	if (addresses == NULL)
	{
		g_fetch_mode = 1;
		return;
	}
	ssize_t ret = getline(&line, &len, addresses);
	if (ret < 0)
	{
		perror("setup_fetch");
		exit(EXIT_FAILURE);
	}
	char *token = strtok(line, " ");
	size_t i = 0;
	while (token != NULL)
	{
		g_route[i++] = (void *)strtoul(token, NULL, 16);
		token = strtok(NULL, " ");
	}
	g_route[i] = NULL;
	free(line);
	fclose(addresses);
	
	FILE *iteration = fopen(ITER_PATH, "r");
	if (iteration != NULL)
	{
		ssize_t ret = getline(&line, &len, iteration);
		if (ret >= 0)
			g_iteration = strtoul(line, NULL, 10);
	}
	g_fetch_mode = 0;
}

void *fetch_mode(size_t size, void *route[20])
{
	void *result = malloc(size);
	t_alloc *found = find_alloc_vector(&g_alloc_vector, route);
	if (!found)
	{
		t_alloc alloc;

		alloc.size = size;
		route_copy(alloc.route, route);
		push_back_vector(&g_alloc_vector, &alloc);
	}
	else
	{
		found->iteration++;
		found->size += size;
	}
	return (result);
}

void *block_mode(size_t size, void *route[20])
{
	void 	*result;
	static size_t 	iteration = 0;

	if (route_eq_stack(route, g_route))
	{
		if (g_iteration == iteration)
			return (NULL);
		else
		{
			iteration++;
			result = malloc(size);
		}
	}
	else
		result = malloc(size);
	return (result);
}

void *alloc_hook(size_t size)
{
	char *result = NULL;
	void *route[100];

	g_malloc_hook_active = 0;
	
	if (!g_at_exit_hook_active)
	{
		g_at_exit_hook_active = 1;
		atexit(at_exit_hook);
	}

	if (g_fetch_mode == -1)
		setup_fetch();

	if (g_fetch_mode && !g_alloc_vector_setup)
	{
		g_alloc_vector_setup = 1;
		init_alloc_vector(&g_alloc_vector);
	}

	get_backtrace(route);
	if (g_fetch_mode)
		result = fetch_mode(size, route);
	else
		result = block_mode(size, route);
	if (result)
		add_alloc_list(&g_alloc_list, result, size, route);
	g_malloc_hook_active = 1;
	return (result);
}

void *malloc(size_t size)
{
	void *caller;

	caller = __builtin_return_address(0);
	if (g_malloc_hook_active && (caller < (void*)&_end))
		return (alloc_hook(size));
	return (__libc_malloc(size));
}

void *calloc(size_t nmemb, size_t size)
{
	void *caller;

	caller = __builtin_return_address(0);
	if (g_malloc_hook_active && (caller < (void*)&_end))
	{
		char *result = alloc_hook(nmemb * size);
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
