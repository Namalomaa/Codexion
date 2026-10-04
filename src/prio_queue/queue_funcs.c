# include "codexion.h"

static void swap_requests(t_request *a, t_request *b)
{
    t_request temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

static int higher_priority(t_request *a, t_request *b, t_coders *coders)
{
    if (strcmp(coders->scheduler, "fifo") == 0)
        return (a->sequence < b->sequence);

    if (a->deadline != b->deadline)
        return (a->deadline < b->deadline);
    return (a->sequence < b->sequence);
}

static void heapify_up(t_queue *queue, int i, t_coders *coders)
{
    int parent;

    if (i == 0)
        return ;
    parent = (i - 1) / 2;
    if (higher_priority(&queue->request[i], &queue->request[parent], coders))
    {
        swap_requests(&queue->request[i], &queue->request[parent]);
        heapify_up(queue, parent, coders);
    }
}


static void heapify_down(t_queue *queue, int current_idx, t_coders *coders)
{
    int     left;
    int     right;
    int     best;

    left = 2 * current_idx + 1;
    right = 2 * current_idx + 2;
    best = current_idx;
    // check if left child exists and has higher priority than current node
    if (left < queue->size && higher_priority(&queue->request[left], &queue->request[best], coders))
        best = left;
    // check if right child exists and has higher priority than current node
    if (right < queue->size && higher_priority(&queue->request[right], &queue->request[best], coders))
        best = right;
    // just checking if the largest is not the current node, if so swap and heapify down again
    if (best != current_idx)
    {
        swap_requests(&queue->request[current_idx], &queue->request[best]);
        heapify_down(queue, best, coders);
    }
}

int	init_queue(t_queue *queue, t_coders *coders)
{
	queue->request = malloc(sizeof(t_request) * coders->number_of_coders);
	if (!queue->request)
		return (0);
	queue->size = 0;
	queue->capacity = coders->number_of_coders;
	return (1);
}

int push_to_heap(t_queue *queue, t_coder *coder)
{
    t_coders   *coders;
    int         i;

    coders = coder->simulation;
    if (queue->size >= queue->capacity)
        return (0);
    i = queue->size;
    init_request(&queue->request[queue->size], coder);
    printf("REQ   coder=%d seq=%d\n",
        coder->id, queue->request[i].sequence);
    queue->size ++;
    heapify_up(queue, i, coders);
    return (1);
}

void    pop_heap(t_queue *queue, t_coders *coders)
{
    if (queue->size == 0)
        return ;
    // replace the root of the heap with the last element
    queue->request[0] = queue->request[queue->size - 1];
    queue->size --;
    if (queue->size > 0)
        heapify_down(queue, 0, coders);
}
