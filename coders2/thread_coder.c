/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_coder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:54:13 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/05 10:26:40 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_threads(t_coder *coders, size_t capacity, t_monitor *monitor)
{
	size_t	i;

	if (!coders || !monitor)
		return ;
	i = 0;
	coders->inputs->start = time_in_ms();
	pthread_create(&monitor->monitor_id, NULL, monitor_routine, monitor);
	while (i < capacity)
	{
		pthread_create(&coders[i].coder_id, NULL, routine, &coders[i]);
		i++;
	}
	i = 0;
	while (i < capacity)
	{
		pthread_join(coders[i].coder_id, NULL);
		i++;
	}
	pthread_join(monitor->monitor_id, NULL);
	pthread_mutex_destroy(coders[0].print_lock);
}
