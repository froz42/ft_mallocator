/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 14:02:02 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/26 16:51:27 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <stdlib.h>

typedef struct pair_s
{
	size_t data;
	void *caller;
}				pair_t;

typedef struct vector_s
{
	pair_t *data;
	size_t size;
	size_t capacity;
} vector_t;

extern int g_fd_out;

void init_vector(vector_t *vector);
void push_back_vector(vector_t *vector, pair_t *value);
void free_vector(vector_t *vector);
pair_t *find_vector(vector_t *vector, pair_t *value);
void print_vector(vector_t *vector);

#endif
