/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namaloma <namaloma@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:25:37 by namaloma          #+#    #+#             */
/*   Updated: 2026/10/07 18:25:40 by namaloma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_current_time(void)
{
	struct timespec	time;

	clock_gettime(CLOCK_MONOTONIC, &time);
	return (time.tv_sec * 1000 + time.tv_nsec / 1000000);
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
