/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 11:10:11 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/05 12:11:26 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	get_ordered_dongles(t_coder *coder, t_dongle **first,
		t_dongle **second)
{
	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
	{
		*first = coder->left_dongle;
		*second = coder->right_dongle;
	}
	else
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
}

static int	try_acquire(t_coder *coder, t_dongle *first, t_dongle *second)
{
	long long	first_ready;
	long long	second_ready;

	first_ready = first->last_compile_time + coder->inputs->dongle_cooldown;
	second_ready = second->last_compile_time + coder->inputs->dongle_cooldown;
	if (first->status == 0 && second->status == 0
		&& first_ready <= time_in_ms() && second_ready <= time_in_ms()
		&& first->request->arr[0].id == coder->id
		&& second->request->arr[0].id == coder->id)
	{
		pop_coder(first->request);
		pop_coder(second->request);
		first->status = 1;
		second->status = 1;
		return (1);
	}
	return (0);
}

static int	poll_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	while (!is_stopped(coder->inputs))
	{
		pthread_mutex_lock(&first->lock);
		pthread_mutex_lock(&second->lock);
		if (try_acquire(coder, first, second))
		{
			pthread_mutex_unlock(&second->lock);
			pthread_mutex_unlock(&first->lock);
			ft_print("has taken a dongle", coder, 0);
			ft_print("has taken a dongle", coder, 0);
			return (1);
		}
		pthread_mutex_unlock(&second->lock);
		pthread_mutex_unlock(&first->lock);
		usleep(1000);
	}
	return (0);
}

int	take_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	get_ordered_dongles(coder, &first, &second);
	return (poll_dongles(coder, first, second));
}

void	release_dongle(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	get_ordered_dongles(coder, &first, &second);
	pthread_mutex_lock(&first->lock);
	pthread_mutex_lock(&second->lock);
	first->status = 0;
	second->status = 0;
	pthread_cond_broadcast(&first->cond);
	pthread_cond_broadcast(&second->cond);
	pthread_mutex_unlock(&second->lock);
	pthread_mutex_unlock(&first->lock);
}
