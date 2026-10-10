/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   main.c                                            :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: namaloma <namaloma@learner.42.tech>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/09/14 19:25:10 by namaloma         #+#    #+#              */
/*   Updated: 2026/10/07 17:31:00 by namaloma        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_threads(t_coders *coders)
{
	int	i;

	i = 0;
	while (i < coders->number_of_coders)
	{
		pthread_create(&coders->coders[i].thread, NULL,
			coder_protocol, &coders->coders[i]);
		i++;
	}
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
	while (++i < coders.number_of_coders)
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
