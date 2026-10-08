/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namaloma <namaloma@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:25:17 by namaloma          #+#    #+#             */
/*   Updated: 2026/10/07 18:25:19 by namaloma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	print_state(t_coders *coders, int id, char *msg)
{
	int	done;
	int	count;

	if (strcmp(msg, "is compiling") == 0)
		count = get_compile_count(&coders->coders[id - 1]);
	pthread_mutex_lock(&coders->simulation_lock);
	done = 0;
	if (!coders->stop)
	{
		printf("%ld %i %s",
			get_current_time() - coders->start_time, id, msg);
		if (strcmp(msg, "is compiling") == 0)
			printf(" [%d/%d]",
				count + 1,
				coders->number_of_compiles_required);
		printf("\n");
		done = 1;
	}
	pthread_mutex_unlock(&coders->simulation_lock);
	return (done);
}

void	calc_timeout(long duration, struct timespec *timeout)
{
	clock_gettime(CLOCK_REALTIME, timeout);
	timeout->tv_sec += duration / 1000;
	timeout->tv_nsec += (duration % 1000) * 1000000;
	if (timeout->tv_nsec >= 1000000000)
	{
		timeout->tv_sec += 1;
		timeout->tv_nsec -= 1000000000;
	}
}

long	max(long a, long b)
{
	if (a > b)
		return (a);
	return (b);
}

void	fill_coders(t_coders *coders, long *numeric, char *scheduler)
{
	coders->time_to_burnout = numeric[1];
	coders->time_to_compile = numeric[2];
	coders->time_to_debug = numeric[3];
	coders->time_to_refactor = numeric[4];
	coders->number_of_compiles_required = numeric[5];
	coders->dongle_cooldown = numeric[6];
	coders->scheduler = ft_strdup(scheduler);
	coders->next_sequence = 0;
}

int	copy_arg(char *all, char *arg, int idx)
{
	int	j;

	j = 0;
	while (arg[j])
		all[idx++] = arg[j++];
	return (idx);
}
