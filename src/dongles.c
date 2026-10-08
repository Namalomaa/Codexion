/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   dongles.c                                         :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: namaloma <namaloma@learner.42.tech>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/07 17:33:17 by namaloma         #+#    #+#              */
/*   Updated: 2026/10/08 17:24:41 by namaloma        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	try_grant_req(t_coders *coders)
{
	t_request	*request;
	int			index;

	if (coders->queue.size == 0)
		return ;
	while (coders->queue.size > 0)
	{
		index = find_best_available(&coders->queue, coders);
		if (index == -1)
			break ;
		request = &coders->queue.request[index];
		reserve_dongles(request->coder);
		printf("GRANT coder=%d seq=%d deadline=%ld\n",
			request->coder->id,
		request->sequence,
		request->deadline);
		pop_heap_at(&coders->queue, index, coders);
		pthread_cond_broadcast(&coders->scheduler_cond);
	}
}

int	dongle_is_available(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;
	long		now;

	left = coder->left_dongle;
	right = coder->right_dongle;
	now = get_current_time();
	return (
	left->reserved == 0
	&& right->reserved == 0
	&& now >= left->cold_at
	&& now >= right->cold_at
);
}

void	reserve_dongles(t_coder *coder)
{
	coder->right_dongle->reserved = 1;
	coder->left_dongle->reserved = 1;
	coder->granted = 1;
}

static void	release_dongles(t_coder *coder)
{
	long	now;

	now = get_current_time();
	coder->right_dongle->reserved = 0;
	coder->right_dongle->cold_at = now + coder->simulation->dongle_cooldown;
	coder->left_dongle->reserved = 0;
	coder->left_dongle->cold_at = now + coder->simulation->dongle_cooldown;
	coder->granted = 0;
}

void	release_resources(t_coder *coder)
{
	t_coders	*coders;

	coders = coder->simulation;
	pthread_mutex_lock(&coders->scheduler_lock);
	release_dongles(coder);
	try_grant_req(coders);
	pthread_cond_broadcast(&coders->scheduler_cond);
	pthread_mutex_unlock(&coders->scheduler_lock);
}
