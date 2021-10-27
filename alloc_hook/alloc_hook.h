/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_hook.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatis <tmatis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/25 13:28:41 by tmatis            #+#    #+#             */
/*   Updated: 2021/10/27 17:15:59 by tmatis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOC_HOOK_H
# define ALLOC_HOOK_H

# ifndef FETCH_ADDR
#  define FETCH_ADDR 0
# endif

# include <unistd.h>

extern void	*__libc_malloc(size_t size);
extern void	*__libc_calloc(size_t size, size_t nmemb);
extern void	__libc_free(void *ptr);
extern void *_end;

#endif
