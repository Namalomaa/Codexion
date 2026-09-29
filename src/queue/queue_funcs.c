# include "codexion.h"

int	init_queue(t_queue *queue, t_coders *coders)
{
	queue->request = malloc(sizeof(t_request) * coders->number_of_coders);
	if (!queue->request)
		return (0);
	queue->size = 0;
	queue->capacity = coders->number_of_coders;
	return (1);
}

void    enqueue(t_queue *queue, t_coder *coder)
{
    t_coders   *coders;
    int         i;

    coders = coder->simulation;
    if (queue->size >= queue->capacity)
        return ;
    init_request(&queue->request[queue->size], coder);
    queue->size ++;
}

void    dequeue_at(t_queue *queue, int index)
{
    int i;

    if (queue->size == 0)
        return ;
    if (index < 0 || index >= queue->size)
        return ;
    i = index + 1;
    while (i < queue->size)
    {
        queue->request[i - 1] = queue->request[i];
        i ++;
    }
    queue->size --;
}
