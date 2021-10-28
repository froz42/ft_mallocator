/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 19:48:25 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/28 19:38:30 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
	if (ptr == NULL)
		return (1);

	strcpy(ptr, "Hello World");
	free(ptr);

	void *table[10];
	for (int i = 0; i < 10; i++)
	{
		table[i] = malloc(1000);
		if (table[i] == NULL)
		{
			for (int j = 0; j < i; j++)
				free(table[j]);
			return (1);
		}
	}
	for (int i = 0; i < 10; i++)
		free(table[i]);
}
