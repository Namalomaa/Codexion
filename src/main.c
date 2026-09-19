/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namaloma <namaloma@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 19:25:10 by namaloma          #+#    #+#             */
/*   Updated: 2026/09/14 19:25:17 by namaloma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void init_coders(long *numeric, char *scheduler, t_coders *coders)
{
	int i;

	i = 0;
	coders->number_of_coders = numeric[0];
	while (i < coders->number_of_coders)
	{
		coders->coders[i].id = i + 1;
		coders->coders[i].compile_count = 0;
		coders->coders[i].last_compile_start = 0;
		i++;
	}

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
	printf("# of coders: %d\n", coders.number_of_coders);
	printf("Time to burnout: %d\n", coders.time_to_burnout);
	printf("Time to compile: %d\n", coders.time_to_compile);
	printf("number_of_compiles_required: %d\n", coders.number_of_compiles_required);
	printf("dongle_cooldown: %d\n", coders.dongle_cooldown);
	printf("scheduler: %s\n", coders.scheduler);
	free(coders.coders);
	free(coders.scheduler);
	return (0);
}
