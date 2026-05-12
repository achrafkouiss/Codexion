/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_coder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:54:13 by akouiss           #+#    #+#             */
/*   Updated: 2026/05/12 15:47:02 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void create_threads(t_coder *coders, size_t capacity)
{
    size_t i;

    i = 0;
    pthread_mutex_init(coders[0].lock2, NULL);
    while (i < capacity)
    {
        pthread_create(&coders[i].coder_id, NULL, routine, &coders[i]);
        i++;
    }
    i = 0;
    while (i < capacity)
    {
        pthread_join(coders[i].coder_id, NULL);
        i++;
    }
}