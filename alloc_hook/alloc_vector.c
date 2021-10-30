/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_vector.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 22:32:22 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/30 13:30:18 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "alloc_vector.h"
#include <string.h>
#include <stdio.h>
#include "utils_hook.h"
#include "alloc_hook.h"

void	init_alloc_vector(t_alloc_vector *alloc_vector)
{
	alloc_vector->size = 0;
	alloc_vector->capacity = 10;
	alloc_vector->data = malloc(sizeof(*alloc_vector->data) * alloc_vector->capacity);
	
	if (!alloc_vector->data)
	{
		perror("init_alloc_vector");
		exit(EXIT_FAILURE);
	}
}

void push_back_vector(t_alloc_vector *alloc_vector, t_alloc *alloc)
{
	if (alloc_vector->size == alloc_vector->capacity)
	{
		alloc_vector->capacity *= 2;
		alloc_vector->data = realloc(alloc_vector->data, sizeof(*alloc_vector->data) * alloc_vector->capacity);
		if (!alloc_vector->data)
		{
			perror("push_back_vector");
			exit(EXIT_FAILURE);
		}
	}
	size_t i;
	for (i = 0; alloc->route[i]; i++)
		alloc_vector->data[alloc_vector->size].route[i] = alloc->route[i];
	alloc_vector->data[alloc_vector->size].route[i] = NULL;
	alloc_vector->data[alloc_vector->size].iteration = 1;
	alloc_vector->data[alloc_vector->size].size = alloc->size;
	alloc_vector->size++;
}

t_alloc	*find_alloc_vector(t_alloc_vector *alloc_vector, void *route[])
{
	size_t i;
	for (i = 0; i < alloc_vector->size; i++)
	{
		if (routes_eq(alloc_vector->data[i].route, route))
			return (&alloc_vector->data[i]);
	}
	return (NULL);
}

void clear_alloc_vector(t_alloc_vector *alloc_vector)
{
	free(alloc_vector->data);
}

void print_alloc_vector(t_alloc_vector *alloc_vector, int fd)
{
	
	for (size_t i = 0; i < alloc_vector->size; i++)
	{
		for (size_t j = 0; alloc_vector->data[i].route[j]; j++)
			dprintf(fd, "%p:%s ", (void *) ((size_t)&_end - (size_t)alloc_vector->data[i].route[j]),
						get_func_name(alloc_vector->data[i].route[j]));
		dprintf(fd, "#%zd", alloc_vector->data[i].iteration);
		dprintf(fd, "\n");
	}
}
