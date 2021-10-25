/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 14:02:02 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/25 14:22:10 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <stdlib.h>

typedef struct vector_s
{
	size_t *data;
	size_t size;
	size_t capacity;
} vector_t;

void init_vector(vector_t *vector);
void push_back_vector(vector_t *vector, size_t value);
void free_vector(vector_t *vector);
size_t *find_vector(vector_t *vector, size_t value);
void print_vector(vector_t *vector);

#endif
