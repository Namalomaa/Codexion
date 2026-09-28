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

int	init_dongles(t_coders *coders)
{
	int	i;

	coders->dongles = malloc(
			sizeof(t_dongle) * coders->number_of_coders);
	if (!coders->dongles)
		return (0);
	i = 0;
	while (i < coders->number_of_coders)
	{
		pthread_mutex_init(&coders->dongles[i].dongle, NULL);
		coders->coders[i].right_dongle = &coders->dongles[i];
		if (coders->number_of_coders == 1)
			coders->coders[i].left_dongle = NULL;
		else
		{
		coders->coders[i].left_dongle = &coders->dongles[
			(i + 1) % coders->number_of_coders];
		}
		coders->coders[i].simulation = coders;
        coders->dongles[i].cold_at = coders->start_time;
		i ++;
	}
	pthread_cond_init(&coders->simulation_cond, NULL);
	pthread_mutex_init(&coders->simulation_lock, NULL);
	return (1);
}

void	create_threads(t_coders *coders)
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
	{
		pthread_join(coders.coders[i].thread, NULL);
		pthread_mutex_destroy(&coders.dongles[i].dongle);
	}
	pthread_join(coders.monitor_thread, NULL);
	pthread_cond_destroy(&coders.simulation_cond);
	pthread_mutex_destroy(&coders.simulation_lock);
	free(coders.coders);
	free(coders.scheduler);
	free(coders.dongles);
	return (0);
}
