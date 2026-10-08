/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   queue_funcs.c                                     :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: namaloma <namaloma@learner.42.tech>       #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/10/07 17:30:17 by namaloma         #+#    #+#              */
/*   Updated: 2026/10/07 17:30:18 by namaloma        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_queue(t_queue *queue, t_coders *coders)
{
	queue->request = malloc(sizeof(t_request) * coders->number_of_coders);
	if (!queue->request)
		return (0);
	queue->size = 0;
	queue->capacity = coders->number_of_coders;
	return (1);
}

int	find_best_available(t_queue *queue, t_coders *coders)
{
	int	i;
	int	best;

	i = 0;
	best = -1;
	while (i < queue->size)
	{
		if (dongle_is_available(queue->request[i].coder))
		{
			if (best == -1
				|| higher_priority(&queue->request[i],
					&queue->request[best], coders))
				best = i;
		}
		i++;
	}
	return (best);
}

int	push_to_heap(t_queue *queue, t_coder *coder)
{
	t_coders	*coders;
	int			i;

	coders = coder->simulation;
	if (queue->size >= queue->capacity)
		return (0);
	i = queue->size;
	init_request(&queue->request[queue->size], coder);
	printf("REQ   coder=%d seq=%d\n deadline=%ld\n",
		coder->id, queue->request[i].sequence, queue->request[i].deadline);
	queue->size++;
	heapify_up(queue, i, coders);
	return (1);
}

void	pop_heap_at(t_queue *queue, int index, t_coders *coders)
{

	int	parent;

	if (index < 0 || index >= queue->size)
		return ;

	queue->request[index] = queue->request[queue->size - 1];
	queue->size--;

	if (index >= queue->size)
		return ;

	parent = (index - 1) / 2;
	if (index > 0
		&& higher_priority(&queue->request[index],
			&queue->request[parent], coders))
		heapify_up(queue, index, coders);
	else
		heapify_down(queue, index, coders);
}
