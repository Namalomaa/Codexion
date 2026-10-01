/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namaloma <namaloma@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 19:25:10 by namaloma          #+#    #+#             */
/*   Updated: 2026/09/20 14:52:57 by namaloma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int	init_request(t_request *request, t_coder *coder)
{
	if (!request || !coder)
		return (0);
	request->deadline = get_last_compile_start(coder) + coder->simulation->time_to_burnout;
	request->coder = coder;
	request->sequence = coder->simulation->next_sequence++;
	request->request_time = get_current_time();
	return (1);
}

int	init_dongles(t_coders *coders)
{
	int	i;

	coders->dongles = malloc(
			sizeof(t_dongle) * coders->number_of_coders);
	if (!coders->dongles)
		return (0);
	i = -1;
	while (++ i < coders->number_of_coders)
	{
		coders->coders[i].right_idx = i;
		coders->coders[i].left_idx = (i + 1) % coders->number_of_coders;
		coders->coders[i].right_dongle = &coders->dongles[i];
		if (coders->number_of_coders == 1)
			coders->coders[i].left_dongle = NULL;
		else
			coders->coders[i].left_dongle = &coders->dongles[coders->coders[i].left_idx];
		coders->coders[i].simulation = coders;
        coders->dongles[i].cold_at = coders->start_time;
		coders->dongles[i].reserved = 0;
		coders->coders[i].granted = 0;
	}
	pthread_mutex_init(&coders->scheduler_lock, NULL);
	pthread_cond_init(&coders->scheduler_cond, NULL);
	pthread_cond_init(&coders->simulation_cond, NULL);
	pthread_mutex_init(&coders->simulation_lock, NULL);
	return (1);
}

static void	create_threads(t_coders *coders)
{
	int	i;

	i = 0;
	while (i < coders->number_of_coders)
	{
		pthread_create(&coders->coders[i].thread, NULL,
			coder_protocol, &coders->coders[i]);
		i ++;
	}
}

void	init_coders(long *numeric, char *scheduler, t_coders *coders)
{
	int		i;

	i = -1;
	coders->number_of_coders = numeric[0];
	coders->coders = malloc(sizeof(t_coder) * coders->number_of_coders);
	if (!coders->coders)
		return ;
	coders->start_time = get_current_time();
	if (! init_dongles(coders))
		return ;
	while (++ i < coders->number_of_coders)
	{
		coders->coders[i].id = i + 1;
		coders->coders[i].compile_count = 0;
		coders->coders[i].last_compile_start = coders->start_time;
		coders->coders[i].granted = 0;
	}
	coders->time_to_burnout = numeric[1];
	coders->time_to_compile = numeric[2];
	coders->time_to_debug = numeric[3];
	coders->time_to_refactor = numeric[4];
	coders->number_of_compiles_required = numeric[5];
	coders->dongle_cooldown = numeric[6];
	coders->scheduler = ft_strdup(scheduler);
	coders->next_sequence = 0;
	if (!init_queue(&coders->queue, coders))
		return ;
	create_threads(coders);
	pthread_create(&coders->monitor_thread, NULL, coders_monitor, coders);
}

int	main(int argc, char **argv)
{
	int			i;
	t_coders	coders;

	memset(&coders, 0, sizeof(t_coders));
	if (!parse_and_validate(argc, argv, &coders))
		return (1);
	i = -1;
	while (++ i < coders.number_of_coders)
		pthread_join(coders.coders[i].thread, NULL);
	pthread_join(coders.monitor_thread, NULL);
	pthread_cond_destroy(&coders.simulation_cond);
	pthread_cond_destroy(&coders.scheduler_cond);
	pthread_mutex_destroy(&coders.scheduler_lock);
	pthread_mutex_destroy(&coders.simulation_lock);
	free(coders.coders);
	free(coders.scheduler);
	free(coders.dongles);
	free(coders.queue.request);
	return (0);
}
