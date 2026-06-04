/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 01:39:49 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/04 17:56:25 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	stop_simulation(t_input *inputs, t_dongle *dongles, size_t n)
{
	size_t	i;

	pthread_mutex_lock(&inputs->stop_lock);
	inputs->stop = 1;
	pthread_mutex_unlock(&inputs->stop_lock);
	i = 0;
	while (i < n)
	{
		pthread_mutex_lock(&dongles[i].lock);
		pthread_cond_broadcast(&dongles[i].cond);
		pthread_mutex_unlock(&dongles[i].lock);
		i++;
	}
}

void	*monitor_routine(void *arg)
{
	t_monitor	*monitor;
	t_coder		*coders;
	size_t		i;
	long long	time_since_compile;
	int			counter;
	int			finished_count;

	monitor = (t_monitor *)arg;
	coders = monitor->coders;
	while (1)
	{
		pthread_mutex_lock(&monitor->monitor_lock);
		finished_count = monitor->finished_count;
		pthread_mutex_unlock(&monitor->monitor_lock);
		if (finished_count == (int)coders[0].inputs->number_of_coders)
			return (NULL);
		i = 0;
		while (i < coders[0].inputs->number_of_coders)
		{
			pthread_mutex_lock(&coders[i].state_lock);
			time_since_compile = time_in_ms() - coders[i].last_compile_time;
			counter = coders[i].counter;
			pthread_mutex_unlock(&coders[i].state_lock);
			if (time_since_compile >= coders[i].inputs->time_to_burnout
				&& (counter != coders[i].inputs->number_of_compiles_required))
			{
				stop_simulation(coders[i].inputs, monitor->dongles,
					coders[i].inputs->number_of_coders);
				ft_print("burned out", &coders[i], 1);
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
