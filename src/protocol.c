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

int take_dongle(t_coder *coder, int idx)
{
    t_coders *coders;

    coders = coder->simulation;
    pthread_mutex_lock(&coders->dongles[idx].dongle);

    while (!get_stop(coders)
        && get_current_time() < coders->dongles[idx].cold_at)
        usleep(1000);

    if (get_stop(coders))
    {
        pthread_mutex_unlock(&coders->dongles[idx].dongle);
        return (0);
    }

    print_state(coders, coder->id, "has taken a dongle");
    return (1);
}

int	pickup_dongles(t_coder *coder)
{
	t_coders	*coders;
	int			left_idx;
	int			right_idx;

	coders = coder->simulation;
	left_idx = coder->id % coders->number_of_coders;
	right_idx = coder->id - 1;
	if (coder->id == coders->number_of_coders)
	{
		if (!take_dongle(coder, right_idx))
			return (0);
		if (!take_dongle(coder, left_idx))
			return (pthread_mutex_unlock(&coder->right_dongle->dongle), 0);
	}
	else
	{
		if (!take_dongle(coder, left_idx))
			return (0);
		if (!take_dongle(coder, right_idx))
			return (pthread_mutex_unlock(&coder->left_dongle->dongle), 0);
	}
	return (1);
}

void	putdown_dongles(t_coder *coder)
{
	t_coders	*coders;
	long		current_time;
	int			right_id;
	int			left_id;

	coders = coder->simulation;
	right_id = coder->id - 1;
	left_id = coder->id % coders->number_of_coders;
	current_time = get_current_time();
	coders->dongles[left_id].cold_at = current_time + coders->dongle_cooldown;
	pthread_mutex_unlock(&coder->left_dongle->dongle);
	current_time = get_current_time();
	coders->dongles[right_id].cold_at = current_time + coders->dongle_cooldown;
	pthread_mutex_unlock(&coder->right_dongle->dongle);
}

void	*one_coder(t_coder *coder)
{
	t_coders	*coders;

	coders = coder->simulation;
	if (!take_dongle(coder, 0))
		return (NULL);
	pthread_mutex_lock(&coders->simulation_lock);
	while (!coders->stop)
		pthread_cond_wait(&coders->simulation_cond,
			&coders->simulation_lock);
	pthread_mutex_unlock(&coders->simulation_lock);
	pthread_mutex_unlock(&coder->right_dongle->dongle);
	return (NULL);
}

void	*coder_protocol(void *arg)
{
	t_coder	*coder;

	coder = arg;
	if (coder->simulation->number_of_coders == 1)
		return (one_coder(coder));
	while (!get_stop(coder->simulation))
	{
		if (!pickup_dongles(coder))
			break ;
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
