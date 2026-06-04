/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akouiss <akouiss@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 14:22:15 by akouiss           #+#    #+#             */
/*   Updated: 2026/06/04 16:19:03 by akouiss          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	time_in_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
	{
		fprintf(stderr, "gettimeofday failed");
		return (-1);
	}
	return ((long long)tv.tv_sec * 1000) + ((long long)tv.tv_usec / 1000);
}

void	timed_sleep(t_coder *coder, long long time)
{
	long long	start;

	start = time_in_ms();
	while ((time_in_ms() - start) < time && !is_stopped(coder->inputs))
	{
		usleep(1000);
		// burnout_check(coder);
	}
}

void	ft_print(char *str, t_coder *coder, int force)
{
	long long	current_time;

	pthread_mutex_lock(coder->print_lock);
	if (!force && is_stopped(coder->inputs))
	{
		pthread_mutex_unlock(coder->print_lock);
		return ;
	}
	current_time = time_in_ms() - coder->inputs->start;
	printf("%lld %ld %s\n", current_time, coder->id, str);
	pthread_mutex_unlock(coder->print_lock);
}

// void	burnout_check(t_coder *coder)
// {
// 	long long	burnout_time;
// 	long long	time;

// 	pthread_mutex_lock(&coder->state_lock);
// 	time = time_in_ms() - coder->last_compile_time;
// 	pthread_mutex_unlock(&coder->state_lock);
// 	if (time > coder->inputs->time_to_burnout)
// 		coder->burnout = 1;
// }

int	is_stopped(t_input *inputs)
{
	int value;

	pthread_mutex_lock(&inputs->stop_lock);
	value = inputs->stop;
	pthread_mutex_unlock(&inputs->stop_lock);
	return (value);
}