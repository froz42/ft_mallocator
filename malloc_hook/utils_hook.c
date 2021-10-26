/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 19:43:18 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/26 19:56:11 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils_hook.h"
#include <stdlib.h>
#include <string.h>

char const *get_func_name(void *addr)
{
	Dl_info info;
	if (dladdr(addr, &info) && info.dli_sname)
		return (info.dli_sname);
	else
		return ("unknown");
}

int	should_ignore(void *caller)
{
	const char *func_name = get_func_name(caller);
	
	char const *ignore_func_name[] =
	{
		"_IO_file_doallocate",
		"_dl_allocate_tls",
		"unknown",
		NULL
	};
	for (size_t i = 0; ignore_func_name[i]; i++)
	{
		if (!strcmp(func_name, ignore_func_name[i]))
			return (1);
	}
	return (0);
}

