/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   monitor.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: namaloma <namaloma@learner.42.tech>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/23 22:50:51 by namaloma         #+#    #+#              */
/*   Updated: 2026/10/07 17:30:08 by namaloma        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	print_burnout(t_coders *coders, int id)
{
	int	done;

	pthread_mutex_lock(&coders->simulation_lock);
	done = 0;
	if (!coders->stop)
	{
		printf("%ld %i burned out\n",
			get_current_time() - coders->start_time, id);
		coders->stop = 1;
		pthread_cond_broadcast(&coders->simulation_cond);
		done = 1;
	}
	pthread_mutex_unlock(&coders->simulation_lock);
	if (done)
	{
		pthread_mutex_lock(&coders->scheduler_lock);
		pthread_cond_broadcast(&coders->scheduler_cond);
		pthread_mutex_unlock(&coders->scheduler_lock);
	}
	return (done);
}

static void	check_coder_states(t_coders *coders, int *all_compiled)
{
	int	i;

	i = 0;
	while (i < coders->number_of_coders)
	{
		if (get_compile_count(
				&coders->coders[i]) >= coders->number_of_compiles_required)
		{
			(*all_compiled)++;
			i++;
			continue ;
		}
		if (get_current_time() - get_last_compile_start(
				&coders->coders[i]) >= coders->time_to_burnout)
		{
			print_burnout(coders, coders->coders[i].id);
			break ;
		}
		i++;
	}
}

void	*coders_monitor(void *arg)
{
	t_coders	*coders;
	int			all_compiled;

	coders = arg;
	while (!get_stop(coders))
	{
		all_compiled = 0;
		check_coder_states(coders, &all_compiled);
		if (all_compiled == coders->number_of_coders)
		{
			set_stop(coders, 1);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
