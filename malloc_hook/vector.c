/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 14:01:55 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/25 22:00:53 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include <stdio.h>
#include "alloc_list.h"

void init_vector(vector_t *vector)
{
	vector->size = 0;
	vector->capacity = 10;
	vector->data = calloc(sizeof(size_t), vector->capacity);
	if (vector->data == NULL)
		exit(EXIT_FAILURE);
}

void push_back_vector(vector_t *vector, pair_t *value)
{
	if (vector->size == vector->capacity)
	{
		vector->capacity *= 2;
		vector->data = realloc(vector->data, sizeof(pair_t) * vector->capacity);
		if (vector->data == NULL)
			exit(EXIT_FAILURE);
	}
	vector->data[vector->size].data = value->data;
	vector->data[vector->size].caller = value->caller;
	vector->size++;
}

void free_vector(vector_t *vector)
{
	free(vector->data);
}

pair_t *find_vector(vector_t *vector, pair_t *value)
{
	pair_t *result = NULL;
	size_t i = 0;

	while (i < vector->size)
	{
		if (vector->data[i].data == value->data)
		{
			result = &vector->data[i];
			break;
		}
		i++;
	}
	return (result);
}

void print_vector(vector_t *vector)
{
	size_t i = 0;
	while (i < vector->size)
	{
		printf("%#zx(%s) ", vector->data[i].data,
			get_func_name(vector->data[i].caller));
		i++;
	}
	printf("\n");
}
