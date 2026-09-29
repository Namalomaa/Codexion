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

int	print_state(t_coders *coders, int id, char *msg)
{
	int	done;

	pthread_mutex_lock(&coders->simulation_lock);
	done = 0;
	if (!coders->stop)
	{
		printf("%ld %i %s\n",
			get_current_time() - coders->start_time, id, msg);
		done = 1;
	}
	pthread_mutex_unlock(&coders->simulation_lock);
	return (done);
}

long	get_current_time(void)
{
	struct timespec	time;

	clock_gettime(CLOCK_MONOTONIC, &time);
	return (time.tv_sec * 1000 + time.tv_nsec / 1000000);
}

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
	while (!get_stop(coder->simulation) && get_current_time() - start < coder->simulation->time_to_debug)
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

void	*one_coder(t_coder *coder)
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
int		request_resources(t_coder *coder)
{
	t_coders	*coders;

	coders = coder->simulation;
	pthread_mutex_lock(&coders->scheduler_lock);

	enqueue(&coder->simulation->queue, coder);
	// try to grant the request.
	while (!coder->granted && !get_stop(coder->simulation))
		pthread_cond_wait(&coder->simulation->scheduler_cond,
			&coder->simulation->scheduler_lock);
	if (get_stop(coders))
			return (0);
	pthread_mutex_unlock(&coders->scheduler_lock);
	return (1);
}

void	*coder_protocol(void *arg)
{
	t_coder	*coder;

	coder = arg;
	if (coder->simulation->number_of_coders == 1)
		return (one_coder(coder));
	while (!get_stop(coder->simulation))
	{
		if (!request_resources(coder))
			break ;
		compile(coder);
		if (get_stop(coder->simulation))
			break ;
		debug(coder);
		if (get_stop(coder->simulation))
			break ;
		refactor(coder);
	}
	return (NULL);
}
