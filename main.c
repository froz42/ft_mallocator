/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 19:48:25 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/30 13:25:33 by tmatis           ###   ########.fr       */
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
	void *array[10];

	for (int i = 0; i < 10; i++)
	{
		array[i] = malloc(1000);
		if (array[i] == NULL)
		{
			printf("Error\n");
			return (0);
		}
		memset(array[i], 0, 1000);
	}
}
