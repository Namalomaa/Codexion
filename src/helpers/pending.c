/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pending.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namaloma <namaloma@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:25:37 by namaloma          #+#    #+#             */
/*   Updated: 2026/10/07 18:25:40 by namaloma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wait_for_dongles(t_coders *coders, t_coder *root)
{
	long			remaining;
	long			wake_at;
	struct timespec	timeout;

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

int	wait_for_resources(t_coders *coders, t_coder *coder)
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

int	request_resources(t_coder *coder)
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
