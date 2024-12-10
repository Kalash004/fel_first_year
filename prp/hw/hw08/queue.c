#include "queue.h"

queue_t *create_queue(int capacity)
{
    queue_t *que = malloc(sizeof(queue_t) * 1);
    void **tmp = malloc(capacity * sizeof(void *));
    que->capacity = capacity;
    que->read = 0;
    que->write = 1;
    que->pntr = tmp;
    return que;
}

void delete_queue(queue_t *queue)
{
    for (size_t cell_id = queue->read; cell_id < queue->write; ++cell_id)
    {
        free(queue->pntr[cell_id]);
    }
    free(queue->pntr);
    free(queue);
}

bool push_to_queue(queue_t *queue, void *data)
{
    if (queue->write == queue->read)
    {
        return false;
    }
    queue->pntr[queue->write] = data;
    ++queue->write;
    if (queue->write > queue->capacity - 1)
        queue->write = 0;

    ++queue->size;
    return true;
}

void *pop_from_queue(queue_t *queue)
{
    void *target = queue->pntr[queue->read];

    queue->pntr[queue->read] = NULL;

    ++queue->read;
    if (queue->read > queue->capacity - 1)
        queue->read = 0;

    --queue->size;
    return target;
}

void *get_from_queue(queue_t *queue, int idx)
{
    int temp = idx + (int)queue->read;
    if (temp > queue->capacity)
    {
        temp = temp % queue->capacity;
    }
    return queue->pntr[temp];
}

int get_queue_size(queue_t *queue)
{
    return queue->size;
}