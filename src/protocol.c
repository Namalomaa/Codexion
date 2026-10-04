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

long	max(long a, long b)
{
	if (a > b)
		return (a);
	return (b);
}

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

long	get_current_time()
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

static void	wait_for_dongles(t_coders *coders, t_coder *root)
{
	long		remaining;
	long		wake_at;
	struct timespec timeout;

	if (root->left_dongle->reserved || root->right_dongle->reserved)
	{
		pthread_cond_wait(&coders->scheduler_cond,
			&coders->scheduler_lock);
		return ;
	}

	wake_at = max(root->left_dongle->cold_at,
		root->right_dongle->cold_at);
	remaining = wake_at - get_current_time();
	if (remaining <= 0)
		return ;
	calc_timeout(remaining, &timeout);
	pthread_cond_timedwait(&coders->scheduler_cond,
		&coders->scheduler_lock, &timeout);
}

static int	wait_for_resources(t_coders *coders, t_coder *coder)
{
	t_coder	*root;

	while (!coder->granted && !get_stop(coders))
	{
		try_grant_req(coders);
		if (coder->granted || get_stop(coders))
			break ;
		if (coders->queue.size == 0)
			break ;
		root = coders->queue.request[0].coder;
		wait_for_dongles(coders, root);
	}
	return (!get_stop(coders) && coder->granted);
}

static int	request_resources(t_coder *coder)
{
	t_coders	*coders;
	int			granted;

	coders = coder->simulation;
	pthread_mutex_lock(&coders->scheduler_lock);
	if (!push_to_heap(&coders->queue, coder))
		return (pthread_mutex_unlock(&coders->scheduler_lock), 0);
	granted = wait_for_resources(coders, coder);
	pthread_mutex_unlock(&coders->scheduler_lock);
	return (granted);
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

		print_state(coder->simulation, coder->id, "has taken a dongle");
		print_state(coder->simulation, coder->id, "has taken a dongle");
		compile(coder);
		release_resources(coder);
		if (get_stop(coder->simulation))
			break ;
		debug(coder);
		if (get_stop(coder->simulation))
			break ;
		refactor(coder);
	}
	return (NULL);
}
