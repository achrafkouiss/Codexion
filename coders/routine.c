/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 16:06:54 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/04 22:12:24 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*request_dongle(t_coder *coder, t_dongle *dongle)
{
	t_dongle	*smallest;
	long prio;

	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
		smallest = coder->left_dongle;
	else
		smallest = coder->right_dongle;
	if (dongle != smallest)
	{
		pthread_mutex_lock(&smallest->lock);
		while (!is_stopped(coder->inputs) && smallest->request->size > 0
			&& smallest->request->arr[0].id != coder->id)
			pthread_cond_wait(&smallest->cond, &smallest->lock);
		pthread_mutex_unlock(&smallest->lock);
	}
	pthread_mutex_lock(&dongle->lock);
	if (!strcmp(coder->inputs->scheduler, "fifo"))
	{
		pthread_mutex_lock(&coder->state_lock);
		coder->priority = time_in_ms();
		pthread_mutex_unlock(&coder->state_lock);
	}
	else
	{
		pthread_mutex_lock(&coder->state_lock);
		coder->priority = coder->last_compile_time + coder->inputs->time_to_burnout;
		pthread_mutex_unlock(&coder->state_lock);
	}
	pthread_mutex_unlock(&dongle->lock);
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

void	release_dongle(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	pthread_mutex_lock(&first->lock);
	pthread_mutex_lock(&second->lock);
	first->status = 0;
	second->status = 0;
	pthread_cond_broadcast(&first->cond);
	pthread_cond_broadcast(&second->cond);
	pthread_mutex_unlock(&second->lock);
	pthread_mutex_unlock(&first->lock);
}

int	take_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	long long	first_ready;
	long long	second_ready;

	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	while (!is_stopped(coder->inputs))
	{
		pthread_mutex_lock(&first->lock);
		pthread_mutex_lock(&second->lock);
		// maybe i should lock the last_compile_time
		first_ready = first->last_compile_time + coder->inputs->dongle_cooldown;
		second_ready = second->last_compile_time
			+ coder->inputs->dongle_cooldown;
		if (first->status == 0 && second->status == 0
			&& first_ready <= time_in_ms() && second_ready <= time_in_ms()
			&& first->request->arr[0].id == coder->id
			&& second->request->arr[0].id == coder->id
		)
		{
			pop_coder(first->request);
			pop_coder(second->request);
			first->status = 1;
			second->status = 1;
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

void	*routine(void *arg)
{
	t_coder	*coder;
	int counter;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->state_lock);
	coder->last_compile_time = time_in_ms();
	pthread_mutex_unlock(&coder->state_lock);
	if (coder->left_dongle == NULL)
	{
		pthread_mutex_lock(&coder->right_dongle->lock);
		ft_print("has taken a dongle", coder, 0);
		while (!is_stopped(coder->inputs))
			usleep(1000);
		pthread_mutex_unlock(&coder->right_dongle->lock);
		return (NULL);
	}
	pthread_mutex_lock(&coder->state_lock);
	counter = coder->counter;
	pthread_mutex_unlock(&coder->state_lock);
	while (counter < coder->inputs->number_of_compiles_required
		&& !is_stopped(coder->inputs))
	{
		check_dongles(coder);
		if (!take_both_dongles(coder))
			// if !is_stopped(coder->inputs) is true it mean he should end i should try to the same thing to check_dongles
			return (NULL);
		if (is_stopped(coder->inputs))
			return (NULL);
		compiling(coder);
		if (is_stopped(coder->inputs))
		{
			release_dongle(coder);
			return (NULL);
		}
		release_dongle(coder);
		debugging(coder);
		refactoring(coder);
		pthread_mutex_lock(&coder->state_lock);
		counter = coder->counter;
		pthread_mutex_unlock(&coder->state_lock);
		
	}
	
	pthread_mutex_lock(&coder->inputs->monitor->monitor_lock);
	coder->inputs->monitor->finished_count++;
	pthread_mutex_unlock(&coder->inputs->monitor->monitor_lock);
	// printf("routine: out here\n");
	// printf("routine: out here\n");
	return (NULL);
}

// i should not forget the max coders i should put a limit