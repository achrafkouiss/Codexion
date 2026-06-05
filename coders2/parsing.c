/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:47:50 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/05 11:57:32 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	ft_atol(char *str)
{
	size_t	i;
	long	nbr;

	i = 0;
	nbr = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			nbr = nbr * 10 + (str[i] - '0');
		else
			return (2147483649);
		if (nbr > 2147483647)
			return (2147483649);
		i++;
	}
	return (nbr);
}

static int	convert_string(char *ac[], t_input *inputs)
{
	inputs->number_of_coders = ft_atol(ac[1]);
	if (inputs->number_of_coders == 0 || inputs->number_of_coders == 2147483649
		|| inputs->number_of_coders > MAX_CODERS)
		return (0);
	inputs->time_to_burnout = ft_atol(ac[2]);
	if (inputs->time_to_burnout == 2147483649)
		return (0);
	inputs->time_to_compile = ft_atol(ac[3]);
	if (inputs->time_to_compile == 2147483649)
		return (0);
	inputs->time_to_debug = ft_atol(ac[4]);
	if (inputs->time_to_debug == 2147483649)
		return (0);
	inputs->time_to_refactor = ft_atol(ac[5]);
	if (inputs->time_to_refactor == 2147483649)
		return (0);
	inputs->number_of_compiles_required = ft_atol(ac[6]);
	if (inputs->number_of_compiles_required == 2147483649)
		return (0);
	inputs->dongle_cooldown = ft_atol(ac[7]);
	if (inputs->dongle_cooldown == 2147483649)
		return (0);
	inputs->scheduler = ac[8];
	return (1);
}

t_input	*parsing(int ac, char *av[])
{
	t_input	*inputs;

	inputs = malloc(sizeof(t_input));
	if (!inputs)
		return (NULL);
	if (ac != 9 || !convert_string(av, inputs) || (strcmp(av[8],
				"edf") & strcmp(av[8], "fifo")))
	{
		fprintf(stderr,
			"==================== INVALID INPUT ====================\n"
			"\nYour program must take the following arguments:\n"
			"- number_of_coders time_to_burnout time_to_compile\n"
			"  time_to_debug time_to_refactor\n"
			"  number_of_compiles_required dongle_cooldown scheduler\n"
			"\nAll numeric inputs must be positive and < 2147483648\n"
			"number_of_coders must be > 0 and <= MAX_CODERS\n"
			"scheduler must be either 'edf' or 'fifo'\n");
		free(inputs);
		return (NULL);
	}
	if (pthread_mutex_init(&inputs->stop_lock, NULL))
		return (free(inputs), NULL);
	inputs->stop = 0;
	return (inputs);
}
