/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_vector.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 22:27:22 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/30 13:12:23 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOC_VECTOR_H
# define ALLOC_VECTOR_H

# include <stdlib.h>

typedef struct s_alloc
{
	void *route[20];
	size_t iteration;
	size_t size;
} t_alloc;

typedef struct s_alloc_vector
{
	t_alloc *data;
	size_t size;
	size_t capacity;
} t_alloc_vector;

void	init_alloc_vector(t_alloc_vector *alloc_vector);
void push_back_vector(t_alloc_vector *alloc_vector, t_alloc *alloc);
t_alloc	*find_alloc_vector(t_alloc_vector *alloc_vector, void *route[]);
void clear_alloc_vector(t_alloc_vector *alloc_vector);
void print_alloc_vector(t_alloc_vector *alloc_vector, int fd);


#endif
