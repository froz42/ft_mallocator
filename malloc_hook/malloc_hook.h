/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_hook.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 13:28:41 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/25 15:25:58 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_HOOK_H
# define MALLOC_HOOK_H

# ifndef FETCH_RUN
#  define FETCH_RUN 1
# endif

# include <unistd.h>

extern void	*__libc_malloc(size_t size);
extern void	__libc_free(void *ptr);
extern void *_end;

#endif
