/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 14:01:55 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/25 14:25:45 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include <stdio.h>

void init_vector(vector_t *vector)
{
	vector->size = 0;
	vector->capacity = 10;
	vector->data = calloc(sizeof(size_t), vector->capacity);
	if (vector->data == NULL)
		exit(EXIT_FAILURE);
}

void push_back_vector(vector_t *vector, size_t value)
{
	if (vector->size == vector->capacity)
	{
		vector->capacity *= 2;
		vector->data = realloc(vector->data, sizeof(size_t) * vector->capacity);
		if (vector->data == NULL)
			exit(EXIT_FAILURE);
	}
	vector->data[vector->size] = value;
	vector->size++;
}

void free_vector(vector_t *vector)
{
	free(vector->data);
}

size_t *find_vector(vector_t *vector, size_t value)
{
	size_t *result = NULL;
	size_t i = 0;

	while (i < vector->size)
	{
		if (vector->data[i] == value)
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
		printf("%#zx ", vector->data[i]);
		i++;
	}
	printf("\n");
}
