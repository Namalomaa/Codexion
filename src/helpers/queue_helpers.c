/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namaloma <namaloma@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 18:24:50 by namaloma          #+#    #+#             */
/*   Updated: 2026/10/07 18:24:53 by namaloma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_requests(t_request *a, t_request *b)
{
	t_request	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	higher_priority(t_request *a, t_request *b, t_coders *coders)
{
	if (strcmp(coders->scheduler, "fifo") == 0)
		return (a->sequence < b->sequence);
	if (a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	return (a->sequence < b->sequence);
}

void	heapify_up(t_queue *queue, int i, t_coders *coders)
{
	int	parent;

	if (i == 0)
		return ;
	parent = (i - 1) / 2;
	if (higher_priority(&queue->request[i], &queue->request[parent], coders))
	{
		swap_requests(&queue->request[i], &queue->request[parent]);
		heapify_up(queue, parent, coders);
	}
}

void	heapify_down(t_queue *queue, int current_idx, t_coders *coders)
{
	int	left;
	int	right;
	int	best;

	left = 2 * current_idx + 1;
	right = 2 * current_idx + 2;
	best = current_idx;
	if (left < queue->size && higher_priority(&queue->request[left],
			&queue->request[best], coders))
		best = left;
	if (right < queue->size && higher_priority(&queue->request[right],
			&queue->request[best], coders))
		best = right;
	if (best != current_idx)
	{
		swap_requests(&queue->request[current_idx], &queue->request[best]);
		heapify_down(queue, best, coders);
	}
}
