/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_resources.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 11:11:23 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/07 02:32:12 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*free_dongles(t_dongle *dongles, size_t size)
{
	size_t	i;

	if (!dongles)
		return (NULL);
	i = 0;
	while (i < size)
	{
		pthread_mutex_destroy(&dongles[i].lock);
		pthread_cond_destroy(&dongles[i].cond);
		if (dongles[i].request)
		{
			free(dongles[i].request->arr);
			pthread_mutex_destroy(&dongles[i].request->lock);
			free(dongles[i].request);
		}
		i++;
	}
	free(dongles);
	return (NULL);
}

void	*free_coders(t_coder *coders, t_dongle *dongles, size_t size)
{
	size_t	i;

	if (!dongles)
		return (NULL);
	i = 0;
	while (i < size)
	{
		pthread_mutex_destroy(&coders[i].state_lock);
		i++;
	}
	free(coders);
	coders = NULL;
	return (free_dongles(dongles, size));
}
