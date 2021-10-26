/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 19:48:25 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/26 23:08:03 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char *malloc_function(void)
{
	return (malloc(1000));
}

void other_func(void)
{
	free(malloc_function());
}

int main(void)
{
	free(malloc_function());
	//free(dest);
	other_func();

	size_t i = 8;
	while (i--)
		free(malloc_function());
}
