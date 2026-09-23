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

void	pickup_dongles(t_coder *coder)
{
	if (coder->id == coder->simulation->number_of_coders)
	{
		pthread_mutex_lock(coder->right_dongle);
		print_state(coder->simulation, coder->id, "has taken a dongle");
		pthread_mutex_lock(coder->left_dongle);
		print_state(coder->simulation, coder->id, "has taken a dongle");
	}
	else
	{
		pthread_mutex_lock(coder->left_dongle);
		print_state(coder->simulation, coder->id, "has taken a dongle");
		pthread_mutex_lock(coder->right_dongle);
		print_state(coder->simulation, coder->id, "has taken a dongle");
	}
}

void	putdown_dongles(t_coder *coder)
{
	pthread_mutex_unlock(coder->left_dongle);
	pthread_mutex_unlock(coder->right_dongle);
}

void	*coder_protocol(void *arg)
{
	t_coder	*coder;

	coder = arg;
	while (!get_stop(coder->simulation))
	{
		pickup_dongles(coder);
		if (get_stop(coder->simulation))
		{
			putdown_dongles(coder);
			break ;
		}
		compile(coder);
		putdown_dongles(coder);
		if (get_stop(coder->simulation))
			break ;
		debug(coder);
		if (get_stop(coder->simulation))
			break ;
		refactor(coder);
	}
	return (NULL);
}
