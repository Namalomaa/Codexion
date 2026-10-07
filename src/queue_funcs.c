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

# include "codexion.h"

int	init_queue(t_queue *queue, t_coders *coders)
{
	queue->request = malloc(sizeof(t_request)*coders->number_of_coders);
	if (!queue->request)
		return (0);
	queue->size = 0;
	queue->capacity = coders->number_of_coders;
	return (1);
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

void	pop_heap(t_queue *queue, t_coders *coders)
{
	if (queue->size == 0)
		return ;
	// replace the root of the heap with the last element
	queue->request[0] = queue->request[queue->size - 1];
	queue->size--;
	if (queue->size > 0)
		heapify_down(queue, 0, coders);
}
