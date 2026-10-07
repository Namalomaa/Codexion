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
