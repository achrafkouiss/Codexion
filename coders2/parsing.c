/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 13:47:50 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/05                                   ###   ########.fr   */
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

static int	print_arg_error(int idx, char *arg, char *msg)
{
	fprintf(stderr, "Error: arg %d %s -> %s\n", idx, msg, arg);
	return (0);
}

static int	convert_string(char *ac[], t_input *in)
{
	in->number_of_coders = ft_atol(ac[1]);
	if (in->number_of_coders == 2147483649)
		return (print_arg_error(1, ac[1], "invalid number"));
	if (in->number_of_coders == 0
		|| in->number_of_coders > MAX_CODERS)
		return (print_arg_error(1, ac[1], "out of range"));
	in->time_to_burnout = ft_atol(ac[2]);
	if (in->time_to_burnout == 2147483649)
		return (print_arg_error(2, ac[2], "invalid number"));
	in->time_to_compile = ft_atol(ac[3]);
	if (in->time_to_compile == 2147483649)
		return (print_arg_error(3, ac[3], "invalid number"));
	in->time_to_debug = ft_atol(ac[4]);
	if (in->time_to_debug == 2147483649)
		return (print_arg_error(4, ac[4], "invalid number"));
	in->time_to_refactor = ft_atol(ac[5]);
	if (in->time_to_refactor == 2147483649)
		return (print_arg_error(5, ac[5], "invalid number"));
	in->number_of_compiles_required = ft_atol(ac[6]);
	if (in->number_of_compiles_required == 2147483649)
		return (print_arg_error(6, ac[6], "invalid number"));
	in->dongle_cooldown = ft_atol(ac[7]);
	if (in->dongle_cooldown == 2147483649)
		return (print_arg_error(7, ac[7], "invalid number"));
	return (1);
}

static void	print_usage(void)
{
	fprintf(stderr, "\n==================== INVALID INPUT ====================\n\n");
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "./codexion <number_of_coders> <time_to_burnout> <time_to_compile>\n");
	fprintf(stderr, "           <time_to_debug> <time_to_refactor> <number_of_compiles>\n");
	fprintf(stderr, "           <dongle_cooldown> <scheduler>\n\n");
	fprintf(stderr, "Constraints:\n");
	fprintf(stderr, "number_of_coders > 0 and <= %d\n", MAX_CODERS);
	fprintf(stderr, "time values >= 0 and < 2147483648\n");
	fprintf(stderr, "scheduler: fifo or edf\n");
}

t_input	*parsing(int ac, char *av[])
{
	t_input	*in;

	if (ac != 9)
		return (print_usage(), NULL);
	in = malloc(sizeof(t_input));
	if (!in)
		return (NULL);
	if (!convert_string(av, in))
		return (free(in), NULL);
	if (strcmp(av[8], "edf") && strcmp(av[8], "fifo"))
	{
		fprintf(stderr, "Error: arg 8 invalid scheduler -> %s\n", av[8]);
		return (free(in), NULL);
	}
	in->scheduler = av[8];
	if (pthread_mutex_init(&in->stop_lock, NULL))
		return (free(in), NULL);
	in->stop = 0;
	return (in);
}