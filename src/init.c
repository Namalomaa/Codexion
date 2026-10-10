/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namaloma <namaloma@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:25:37 by namaloma          #+#    #+#             */
/*   Updated: 2026/10/07 18:25:40 by namaloma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_coders(long *numeric, char *scheduler, t_coders *coders)
{
	int	i;

	i = -1;
	coders->number_of_coders = numeric[0];
	coders->coders = malloc(sizeof(t_coder) * coders->number_of_coders);
	if (!coders->coders)
		return ;
	coders->start_time = get_current_time();
	if (!init_dongles(coders))
		return ;
	while (++i < coders->number_of_coders)
	{
		coders->coders[i].id = i + 1;
		coders->coders[i].compile_count = 0;
		coders->coders[i].last_compile_start = coders->start_time;
		coders->coders[i].granted = 0;
	}
	fill_coders(coders, numeric, scheduler);
	if (!init_queue(&coders->queue, coders))
		return ;
	create_threads(coders);
}

void	init_cond_mux(t_coders *coders)
{
	pthread_mutex_init(&coders->scheduler_lock, NULL);
	pthread_cond_init(&coders->scheduler_cond, NULL);
	pthread_cond_init(&coders->simulation_cond, NULL);
	pthread_mutex_init(&coders->simulation_lock, NULL);
}

void	fill_coders(t_coders *coders, long *numeric, char *scheduler)
{
	coders->time_to_burnout = numeric[1];
	coders->time_to_compile = numeric[2];
	coders->time_to_debug = numeric[3];
	coders->time_to_refactor = numeric[4];
	coders->number_of_compiles_required = numeric[5];
	coders->dongle_cooldown = numeric[6];
	coders->scheduler = ft_strdup(scheduler);
	coders->next_sequence = 0;
}

int	init_dongles(t_coders *coders)
{
	int	i;

	coders->dongles = malloc(sizeof(t_dongle) * coders->number_of_coders);
	if (!coders->dongles)
		return (0);
	i = -1;
	while (++i < coders->number_of_coders)
	{
		coders->coders[i].right_idx = i;
		coders->coders[i].left_idx = (i + 1) % coders->number_of_coders;
		coders->coders[i].right_dongle = &coders->dongles[i];
		if (coders->number_of_coders == 1)
			coders->coders[i].left_dongle = NULL;
		else
			coders->coders[i].left_dongle = (
					&coders->dongles[coders->coders[i].left_idx]);
		coders->coders[i].simulation = coders;
		coders->dongles[i].cold_at = coders->start_time;
		coders->dongles[i].reserved = 0;
		coders->coders[i].granted = 0;
	}
	init_cond_mux(coders);
	return (1);
}
