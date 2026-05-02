/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:47:44 by akouiss           #+#    #+#             */
/*   Updated: 2026/05/01 16:38:15 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void *free_dongles(t_dongle **dongles, long size)
{
	int i;

	i = 0;
	while (i < size)
	{
		free(dongles[i]->request);
		free(dongles[i++]);
	}
	free(dongles);
	dongles = NULL;
	return (NULL);
}

void *free_coders(t_coder **coders, t_dongle **dongles, long len, long size)
{
	int i;

	i = 0;
	while (i < len)
		free(coders[i++]);
	free(coders);
	coders = NULL;
	return (free_dongles(dongles, size));
}


t_dongle **init_dongle(long size)
{
	int i;
	t_dongle **dongles;

	i = 0;
	dongles = malloc(size * sizeof(t_dongle));
	while (i < size)
	{
		dongles[i] = malloc(sizeof(t_dongle));
		if (!dongles[i])
			return free_dongles(dongles, i);
		dongles[i]->dongle_id = i;
		dongles[i]->status = 1;
		dongles[i]->last_compile_time = 0;
		dongles[i]->request = NULL;
		i++;
	}
	return dongles;
}

t_coder **init_coders(t_dongle **dongles, long size)
{
	int i;
	t_coder		**coders;

	if (!dongles)
		return (NULL);
	i = 0;
	coders = malloc(size * sizeof(t_coder));
	while (i < size)
	{
		coders[i] = malloc(sizeof(t_coder));
		if (!coders[i])
			return (free_coders(coders, dongles, i, size));
		coders[i]->counter = 0;
		coders[i]->last_compile_time = 0;
		coders[i]->right_dongle = dongles[i];
		if (size == 1)
			coders[i]->left_dongle = NULL;
		else if (i == size - 1)
			coders[i]->left_dongle = dongles[0];
		else
			coders[i]->left_dongle = dongles[i + 1];
		i++;
	}
	return (coders);
}

int	codexion(int ac, char *av[])
{
	t_input		*inputs;
	t_dongle	**dongles;
	t_coder		**coders;
	// t_monitor	*monitor;

	inputs = parsing(ac, av);
	if (!inputs)
		return (0);
	dongles = init_dongle(inputs->number_of_coders);
	coders = init_coders(dongles, inputs->number_of_coders);
	if (!coders)
		return  (free(inputs), 0);
	for (int i = 0; i < inputs->number_of_coders; i++)
	{
		printf("xxxxxxxxxxxxxxxxxxx\n");
		printf("i = %d\n", i);
		printf("coders[%d] = %ld\n", i, coders[i]->coder_id);
		printf("left = %ld\n", coders[i]->left_dongle->dongle_id);
		printf("right = %ld\n", coders[i]->right_dongle->dongle_id);
	}
	free(inputs);
	return (0);
}

int	main(int ac, char *av[])
{
	int		i;
	t_input	*t_input;

	i = 1;
	codexion(ac, av);
}
