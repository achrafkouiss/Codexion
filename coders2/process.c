/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 01:44:54 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/05 10:18:38 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	lock_unlock_last_compile_time(t_coder *coder, t_dongle *dongle)
{
	if (coder)
	{
		pthread_mutex_lock(&coder->state_lock);
		coder->last_compile_time = time_in_ms();
		pthread_mutex_unlock(&coder->state_lock);
	}
	else
	{
		pthread_mutex_lock(&dongle->lock);
		dongle->last_compile_time = time_in_ms();
		pthread_mutex_unlock(&dongle->lock);
	}
}

void	compiling(t_coder *coder)
{
	ft_print("is compiling", coder, 0);
	pthread_mutex_lock(&coder->state_lock);
	coder->counter++;
	pthread_mutex_unlock(&coder->state_lock);
	lock_unlock_last_compile_time(coder, NULL);
	timed_sleep(coder, coder->inputs->time_to_compile);
	lock_unlock_last_compile_time(NULL, coder->left_dongle);
	lock_unlock_last_compile_time(NULL, coder->right_dongle);
}

void	debugging(t_coder *coder)
{
	ft_print("is debugging", coder, 0);
	timed_sleep(coder, coder->inputs->time_to_debug);
}

void	refactoring(t_coder *coder)
{
	ft_print("is refactoring", coder, 0);
	timed_sleep(coder, coder->inputs->time_to_refactor);
}
