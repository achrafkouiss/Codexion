/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 16:06:54 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/07 06:05:52 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	run_cycle(t_coder *coder)
{
	compiling(coder);
	if (is_stopped(coder->inputs))
	{
		release_dongle(coder);
		return ;
	}
	release_dongle(coder);
	debugging(coder);
	refactoring(coder);
}

static void	*compile_loop(t_coder *coder)
{
	long	counter;

	pthread_mutex_lock(&coder->state_lock);
	counter = coder->counter;
	pthread_mutex_unlock(&coder->state_lock);
	while (counter < coder->inputs->number_of_compiles_required
		&& !is_stopped(coder->inputs))
	{
		check_dongles(coder);
		if (!take_both_dongles(coder))
			return (NULL);
		if (is_stopped(coder->inputs))
			return (NULL);
		run_cycle(coder);
		pthread_mutex_lock(&coder->state_lock);
		counter = coder->counter;
		pthread_mutex_unlock(&coder->state_lock);
	}
	pthread_mutex_lock(&coder->inputs->monitor->monitor_lock);
	coder->inputs->monitor->finished_count++;
	pthread_mutex_unlock(&coder->inputs->monitor->monitor_lock);
	return (NULL);
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	lock_unlock_last_compile_time(coder, NULL);
	if (coder->left_dongle == NULL && coder->inputs->number_of_compiles_required != 0)
	{
		pthread_mutex_lock(&coder->right_dongle->lock);
		ft_print("has taken a dongle", coder, 0);
		while (!is_stopped(coder->inputs))
			usleep(1000);
		pthread_mutex_unlock(&coder->right_dongle->lock);
		return (NULL);
	}
	return (compile_loop(coder));
}
