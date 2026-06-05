/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 11:21:59 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/05 11:55:55 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	codexion(int ac, char *av[])
{
	t_input			*inputs;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_mutex_t	print_lock;
	t_monitor		*monitor;

	inputs = parsing(ac, av);
	if (!inputs)
		return (0);
	dongles = init_dongle(inputs->number_of_coders);
	coders = init_coders(dongles, inputs->number_of_coders, &print_lock,
			inputs);
	monitor = init_monitor(coders, dongles);
	if (!monitor)
		return (free(inputs), 1);
	inputs->monitor = monitor;
	create_threads(coders, inputs->number_of_coders, monitor);
	free_coders(coders, dongles, inputs->number_of_coders);
	pthread_mutex_destroy(&monitor->monitor_lock);
	free(inputs);
	free(monitor);
	return (0);
}

int	main(int ac, char *av[])
{
	codexion(ac, av);
	return (0);
}
