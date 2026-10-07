/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   protocol.c                                        :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: namaloma <namaloma@learner.42.tech>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/23 22:57:45 by namaloma         #+#    #+#              */
/*   Updated: 2026/09/23 23:11:21 by namaloma        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_coder *coder)
{
	long	start;

	start = get_current_time();
	set_last_compile_start(coder, start);
	print_state(coder->simulation, coder->id, "is compiling");
	while (!get_stop(coder->simulation)
		&& get_current_time() - start < coder->simulation->time_to_compile)
	{
		usleep(1000);
	}
	if (!get_stop(coder->simulation))
		set_compile_count(coder, get_compile_count(coder) + 1);
}

void	debug(t_coder *coder)
{
	long	start;

	start = get_current_time();
	print_state(coder->simulation, coder->id, "is debugging");
	while (!get_stop(coder->simulation)
		&& get_current_time() - start < coder->simulation->time_to_debug)
		usleep(1000);
}

void	refactor(t_coder *coder)
{
	long	start;

	start = get_current_time();
	print_state(coder->simulation, coder->id, "is refactoring");
	while (!get_stop(coder->simulation)
		&& get_current_time() - start < coder->simulation->time_to_refactor)
	{
		usleep(1000);
	}
}

static void	*one_coder(t_coder *coder)
{
	t_coders	*coders;

	coders = coder->simulation;
	print_state(coders, 1, "has taken a dongle");
	pthread_mutex_lock(&coders->simulation_lock);
	while (!coders->stop)
		pthread_cond_wait(&coders->simulation_cond,
			&coders->simulation_lock);
	pthread_mutex_unlock(&coders->simulation_lock);
	return (NULL);
}

void	*coder_protocol(void *arg)
{
	t_coder		*coder;
	t_coders	*coders;

	coder = arg;
	coders = coder->simulation;
	if (coders->number_of_coders == 1)
		return (one_coder(coder));
	while (!get_stop(coders))
	{
		if (coder->compile_count >= coders->number_of_compiles_required)
			break ;
		if (!request_resources(coder))
			break ;
		print_state(coders, coder->id, "has taken a dongle");
		print_state(coders, coder->id, "has taken a dongle");
		compile(coder);
		release_resources(coder);
		if (get_stop(coders))
			break ;
		debug(coder);
		if (get_stop(coders))
			break ;
		refactor(coder);
	}
	return (NULL);
}
