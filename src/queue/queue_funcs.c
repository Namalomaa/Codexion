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
    i = queue->size;
    queue->request[i].coder = coder;
    init_request(&queue->request[queue->size], coder);
    queue->size ++;
}

t_request   *dequeue(t_queue *queue, t_request *out)
{
    int i;

    if (queue->size == 0)
        return (NULL);
    *out = queue->request[0]; // first in first out
    i = 1;
    while (i < queue->size)
    {
        queue->request[i - 1] = queue->request[i];
        i ++;
    }
    queue->size --;
    return (out);
}
