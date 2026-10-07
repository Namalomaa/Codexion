/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   protection.c                                      :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: namaloma <namaloma@learner.42.tech>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/23 22:57:02 by namaloma         #+#    #+#              */
/*   Updated: 2026/10/07 17:31:30 by namaloma        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_stop(t_coders *coders)
{
	int	value;

	pthread_mutex_lock(&coders->simulation_lock);
	value = coders->stop;
	pthread_mutex_unlock(&coders->simulation_lock);
	return (value);
}

long	get_last_compile_start(t_coder *coder)
{
	long	value;

	pthread_mutex_lock(&coder->simulation->simulation_lock);
	value = coder->last_compile_start;
	pthread_mutex_unlock(&coder->simulation->simulation_lock);
	return (value);
}

int	get_compile_count(t_coder *coder)
{
	int	value;

	pthread_mutex_lock(&coder->simulation->simulation_lock);
	value = coder->compile_count;
	pthread_mutex_unlock(&coder->simulation->simulation_lock);
	return (value);
}

long	get_current_time(void)
{
	struct timespec	time;

	clock_gettime(CLOCK_MONOTONIC, &time);
	return (time.tv_sec * 1000 + time.tv_nsec / 1000000);
}
