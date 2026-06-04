/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 01:44:54 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/04 21:51:31 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// void lock_unlock_last_compile_time(t_coder *coder, t_dongle *dongle)
// {

// 	if (coder)
// 	{
// 		pthread_mutex_lock(&coder->state_lock);
// 		coder->last_compile_time = time_in_ms();
// 		pthread_mutex_unlock(&coder->state_lock);
// 	}
// 	else
// 	{
// 		pthread_mutex_lock(&dongle->lock);
// 		dongle->last_compile_time = time_in_ms();
// 		pthread_mutex_unlock(&dongle->lock);
// 	}
// }

void	compiling(t_coder *coder)
{
	// burnout_check(coder);
	ft_print("is compiling", coder, 0);
	pthread_mutex_lock(&coder->state_lock);
	coder->counter++;
	pthread_mutex_unlock(&coder->state_lock);
	// lock_unlock_last_compile_time(coder, NULL);
	pthread_mutex_lock(&coder->state_lock);
	coder->last_compile_time = time_in_ms();
	pthread_mutex_unlock(&coder->state_lock);
	timed_sleep(coder, coder->inputs->time_to_compile);
	// lock_unlock_last_compile_time(NULL, coder->left_dongle);
	pthread_mutex_lock(&coder->left_dongle->lock);
	coder->left_dongle->last_compile_time = time_in_ms();
	pthread_mutex_unlock(&coder->left_dongle->lock);
	// lock_unlock_last_compile_time(NULL, coder->right_dongle);
	pthread_mutex_lock(&coder->right_dongle->lock);
	coder->right_dongle->last_compile_time = time_in_ms();
	pthread_mutex_unlock(&coder->right_dongle->lock);
}


void	debugging(t_coder *coder)
{
	// burnout_check(coder);
	ft_print("is debugging", coder, 0);
	timed_sleep(coder, coder->inputs->time_to_debug);
}

void	refactoring(t_coder *coder)
{
	// burnout_check(coder);
	ft_print("is refactoring", coder, 0);
	timed_sleep(coder, coder->inputs->time_to_refactor);
}