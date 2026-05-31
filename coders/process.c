/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 01:44:54 by akouiss           #+#    #+#             */
/*   Updated: 2026/05/31 01:44:54 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compiling(t_coder *coder)
{
	burnout_check(coder);
	ft_print("is compiling", coder, 0);
	pthread_mutex_lock(&coder->state_lock);
	coder->last_compile_time = time_in_ms();
	pthread_mutex_unlock(&coder->state_lock);
	timed_sleep(coder, coder->inputs->time_to_compile);
	// usleep(coder->inputs->time_to_compile * 1000);
	pthread_mutex_lock(&coder->left_dongle->lock);
	coder->left_dongle->last_compile_time = time_in_ms();
	pthread_mutex_unlock(&coder->left_dongle->lock);
	pthread_mutex_lock(&coder->right_dongle->lock);
	coder->right_dongle->last_compile_time = time_in_ms();
	pthread_mutex_unlock(&coder->right_dongle->lock);
}

void	debugging(t_coder *coder)
{
	burnout_check(coder);
	ft_print("is debugging", coder, 0);
	timed_sleep(coder, coder->inputs->time_to_debug);
	// usleep(coder->inputs->time_to_debug * 1000);
}

void	refactoring(t_coder *coder)
{
	burnout_check(coder);
	ft_print("is refactoring", coder, 0);
	timed_sleep(coder, coder->inputs->time_to_refactor);
	// usleep(coder->inputs->time_to_refactor * 1000);
}