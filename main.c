/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 19:48:25 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/27 16:31:20 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

char *malloc_function(void)
{
	return (malloc(1000));
}

char *other_func(void)
{
	return (malloc_function());
}

int main(void)
{
	char *ptr = malloc_function();
	if (!ptr)
		return (1);

	memset(ptr, 'A', 1000);

	char *ptr2 = malloc_function();
	if (!ptr2)
		return (1);

	memset(ptr2, 'B', 1000);
	
	char *ptr3 = other_func();

	memset(ptr3, 'C', 1000);
}
