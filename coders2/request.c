/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 11:10:11 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/05 12:08:23 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_dongle	*get_smallest(t_coder *coder)
{
	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
		return (coder->left_dongle);
	return (coder->right_dongle);
}

static void	set_priority(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	pthread_mutex_lock(&coder->state_lock);
	if (!strcmp(coder->inputs->scheduler, "fifo"))
		coder->priority = time_in_ms();
	else
		coder->priority = coder->last_compile_time
			+ coder->inputs->time_to_burnout;
	pthread_mutex_unlock(&coder->state_lock);
	pthread_mutex_unlock(&dongle->lock);
}

void	*request_dongle(t_coder *coder, t_dongle *dongle)
{
	t_dongle	*smallest;
	long		prio;

	smallest = get_smallest(coder);
	if (dongle != smallest)
	{
		pthread_mutex_lock(&smallest->lock);
		while (!is_stopped(coder->inputs) && smallest->request->size > 0
			&& smallest->request->arr[0].id != coder->id)
			pthread_cond_wait(&smallest->cond, &smallest->lock);
		pthread_mutex_unlock(&smallest->lock);
	}
	set_priority(coder, dongle);
	pthread_mutex_lock(&coder->state_lock);
	prio = coder->priority;
	pthread_mutex_unlock(&coder->state_lock);
	pthread_mutex_lock(&dongle->request->lock);
	push_coder(coder->id, prio, dongle->request);
	pthread_mutex_unlock(&dongle->request->lock);
	return (NULL);
}

void	check_dongles(t_coder *coder)
{
	if (coder->right_dongle->dongle_id < coder->left_dongle->dongle_id)
	{
		request_dongle(coder, coder->right_dongle);
		request_dongle(coder, coder->left_dongle);
	}
	else
	{
		request_dongle(coder, coder->left_dongle);
		request_dongle(coder, coder->right_dongle);
	}
}
