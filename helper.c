/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 14:22:15 by akouiss           #+#    #+#             */
/*   Updated: 2026/04/28 10:51:52 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void fifo(t_dongle *dongle, t_coder *coder)
{
    t_heap *head;
    t_heap *node;

    
    pthread_mutex_lock(&dongle->lock);
    node = malloc(sizeof(t_heap));
    if (!node)
    node->priority = gettimeofday();
    node->coder = coder;
    node->next_node = dongle->request;
    dongle->request = node;
}

void request_dongle(t_coder *coder)
{
    t_dongle *left_dongle;
    t_dongle *right_dongle;

    pthread_mutex_init(&left_dongle->lock, NULL);
    pthread_mutex_init(&right_dongle->lock, NULL);
    left_dongle = coder->left_dongle;
    right_dongle = coder->right_dongle;
    if (right_dongle->dongle_id < left_dongle->dongle_id && right_dongle->status)
    {
        right_dongle->status = 1;
    }
}

int main()
{
    return (0);
}