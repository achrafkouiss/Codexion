/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:47:50 by akouiss           #+#    #+#             */
/*   Updated: 2026/04/21 15:05:29 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	ft_atol(char *str)
{
	size_t	i;
	long	nbr;

	i = 0;
	nbr = 0;
	while (str[i] == 32 || (str[i] >= 9 & str[i] <= 13))
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
	if (inputs->number_of_coders == 0 || inputs->number_of_coders == 2147483649)
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
			"==================== INVALID INPUT ==============================\n\
            \nYour program must take the following arguments (all mandatory):\
            \n- number_of_coders time_to_burnout time_to_compile time_to_debug\
            \n  time_to_refactor number_of_compiles_required dongle_cooldown scheduler\n\
            \nthe fisrt seven inputs should all be positive and less then 2147483648\
            \nfor the number_of_coders be more than 0\
            \nfor the scheduler should be either edf or fifo");
		free(inputs);
		return (NULL);
	}
	return (inputs);
}
