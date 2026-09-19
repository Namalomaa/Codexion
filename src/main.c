/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namaloma <namaloma@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 19:25:10 by namaloma          #+#    #+#             */
/*   Updated: 2026/09/19 15:11:14 by namaloma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_dongles(t_coders *coders)
{
	int	i;

	coders->dongles = malloc(sizeof(pthread_mutex_t) * coders->number_of_coders);
	if (!coders->coders)
		return;
	i = 0;
	while(i < coders->number_of_coders)
	{
		coders->coders[i].right_dongle = &coders->dongles[i];
		coders->coders[i].left_dongle = &coders->dongles[(i + 1) % coders->number_of_coders];
	}
}

void	init_coders(long *numeric, char *scheduler, t_coders *coders)
{
	int i;

	i = -1;
	coders->number_of_coders = numeric[0];
	coders->coders = malloc(sizeof(t_coder) * coders->number_of_coders);
	if (!coders->coders)
		return;
	while (++ i < coders->number_of_coders)
	{
		coders->coders[i].id = i + 1;
		coders->coders[i].compile_count = 0;
		coders->coders[i].last_compile_start = 0;
        pthread_create(&coders->coders[i].thread, NULL,
			coder_routine, &coders->coders[i]);
		pthread_mutex_init(coders->dongles, NULL);
	}
	init_dongles(coders);
    coders->time_to_burnout = numeric[1];
    coders->time_to_compile = numeric[2];
    coders->time_to_debug = numeric[3];
    coders->time_to_refactor = numeric[4];
    coders->number_of_compiles_required = numeric[5];
    coders->dongle_cooldown = numeric[6];
    coders->scheduler = ft_strdup(scheduler);
}

int	main(int argc, char **argv)
{
	t_coders	coders;

    memset(&coders, 0, sizeof(t_coders));
    if (!parse_and_validate(argc, argv, &coders))
		return (1);
	free(coders.coders);
	free(coders.scheduler);
	return (0);
}
