/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   protection.c                                       :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+      */
/*   By: namaloma <namaloma@learner.42.tech>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/23 22:57:02 by namaloma         #+#    #+#              */
/*   Updated: 2026/09/29 13:51:05 by namaloma         ###   ########.fr       */
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

void	set_last_compile_start(t_coder *coder, long value)
{
	pthread_mutex_lock(&coder->simulation->simulation_lock);
	coder->last_compile_start = value;
	pthread_mutex_unlock(&coder->simulation->simulation_lock);
}
    // try to grant the access;

void	set_compile_count(t_coder *coder, int value)
{
	pthread_mutex_lock(&coder->simulation->simulation_lock);
	coder->compile_count = value;
	pthread_mutex_unlock(&coder->simulation->simulation_lock);
}

void	set_stop(t_coders *coders, int value)
{
	pthread_mutex_lock(&coders->simulation_lock);
	coders->stop = value;
	pthread_cond_broadcast(&coders->simulation_cond);
    pthread_mutex_unlock(&coders->simulation_lock);
	pthread_mutex_lock(&coders->scheduler_lock);
	pthread_cond_broadcast(&coders->scheduler_cond);
	pthread_mutex_unlock(&coders->scheduler_lock);

}
